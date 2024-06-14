#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WinApp.h"

//コントローラーで動くか確認

Player::Player() {}

Player::~Player() {
	// bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle,const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変数の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3Dreticle_.Initialize();
	worldTransform3Dreticle_.translation_ = GetWorldPosition();

	textureHandleReticle_ = TextureManager::Load("reticle.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureHandleReticle_, {640.0f,360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

}


void Player::Update(ViewProjection& viewProjection) {
	//--------------------------------
	// デスフラグの立った弾を削除
	//--------------------------------

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//--------------------------------
	// キーボード入力によって移動ベクトルを変更する処理
	//--------------------------------

	//ActiveKeyboard();

	//--------------------------------
	// ゲームパッド入力によって移動ベクトルを変更する処理
	//--------------------------------

	ActiveGamePad();

	//--------------------------------
	// 移動制限
	//--------------------------------

	LimitMovement();

	//--------------------------------
	// 旋回
	//--------------------------------

	Rotate();

	//--------------------------------
	// 攻撃処理
	//--------------------------------

	Attack();

	//弾を更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//--------------------------------
	// レティクルの配置
	//--------------------------------
	
	Reticle(viewProjection);

	//--------------------------------
	// マウスカーソルの位置にレティクルを配置
	//--------------------------------

	//MouseCursor(viewProjection);
	PadReticle(viewProjection);
	
	//--------------------------------
	//行列の更新
	//--------------------------------
	worldTransform_.UpdateMatrix();

	//--------------------------------
	//ImGuiで座標表示
	//--------------------------------
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -20.0f, 20.0f);	
	ImGui::End();
#endif // _DEBUG

}

void Player::Draw(ViewProjection& viewProjection) { 
	//プレイヤー
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 

	//弾を描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}


void Player::Rotate() {
	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更する
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {

	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

		// 自機から標準オブジェクトへのベクトル
		velocity = GetReticleWorldPosition(worldTransform3Dreticle_) - GetWorldPosition();
		velocity = Normalize(velocity) * kBulletSpeed;

		// 速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {
    //ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::GetReticleWorldPosition(WorldTransform& worldTransform) {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform.matWorld_.m[3][0];
	worldPos.y = worldTransform.matWorld_.m[3][1];
	worldPos.z = worldTransform.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision() {
//何もしない
}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::DrawUI() {
	sprite2DReticle_->Draw();
}

void Player::Reticle(ViewProjection& viewProjection) {
	//--------------------------------
	// 3Dレティクルの配置
	//--------------------------------
	// 自機のワールド座標から3Dレティクルのワールド座標を計算する
	{
		// 自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
		// 時期から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = {0, 0, 1.0f};
		// 自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		// ベクトルの長さを整える
		offset = Normalize(offset) * kDistancePlayerTo3DReticle;
		// 3Dレティクル座標を設定
		worldTransform3Dreticle_.translation_ = GetWorldPosition() + offset;
		worldTransform3Dreticle_.UpdateMatrix();
	}
	//--------------------------------
	// 2Dレティクルの配置
	//--------------------------------
	{
		// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算する

		Vector3 positionReticle = GetReticleWorldPosition(worldTransform3Dreticle_);

		// ビューポート行列
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;

		// ワールド->スクリーン座標変換（ここで3D->2Dになる）
		positionReticle = Transform(positionReticle, matViewProjectionViewport);

		// スプライトのレティクルに座標設定
		//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::SliderFloat3("3Dreticle", &worldTransform3Dreticle_.translation_.x, -20.0f, 20.0f);
	ImGui::End();
}

void Player::MouseCursor(ViewProjection& viewProjection) {
	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置

	POINT mousePosition;

	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	//スクリーン座標はWindowAPI的にはクライアントエリア座標になる
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2{(float)mousePosition.x, (float)mousePosition.y});

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

	//合成行列の逆行列を求める
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3((float)mousePosition.x, (float)mousePosition.y, 0);
	Vector3 posFar = Vector3((float)mousePosition.x, (float)mousePosition.y, 1);

	//スクリーン座標からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posNear, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posNear - posFar;
	mouseDirection = Normalize(mouseDirection);

	//カメラから標準オブジェクトへの距離
	const float kDistanceTestObject = 50.f;
	worldTransform3Dreticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;
	worldTransform3Dreticle_.UpdateMatrix();

	#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3Dreticle_.translation_.x, worldTransform3Dreticle_.translation_.y, worldTransform3Dreticle_.translation_.z);
	ImGui::End();
#endif // _DEBUG

}

void Player::ActiveKeyboard() {
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更する（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更する（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	// 座標移動(ベクトルの加算）
	worldTransform_.translation_ += move;
}

void Player::ActiveGamePad() {
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//ゲームパッドの状態を得る関数(XINPUT)
	XINPUT_STATE joyState;

	//ゲームパッド状態取得
	//SHRT_MAX...short型の最大値。sThumbLXとsThumbLYがshort型なので最大値で割ることで[-1,+1]の範囲に変換している
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算）
	worldTransform_.translation_ += move;

}

void Player::LimitMovement() {
	// 移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 17;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
}

void Player::PadReticle(ViewProjection& viewProjection) {

	//--------------------------------
	// ゲームパッドによるレティクル操作
	//--------------------------------

	// スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;

	const float kReticleSpeed = 5.0f;

	  // ジョイスティック状態を取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kReticleSpeed;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kReticleSpeed;

		// スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

	// 合成行列の逆行列を求める
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3((float)spritePosition.x, (float)spritePosition.y, 0);
	Vector3 posFar = Vector3((float)spritePosition.x, (float)spritePosition.y, 1);

	// スクリーン座標からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posNear, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posNear - posFar;
	mouseDirection = Normalize(mouseDirection);

	// カメラから標準オブジェクトへの距離
	const float kDistanceTestObject = 50.f;
	worldTransform3Dreticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;
	worldTransform3Dreticle_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3Dreticle_.translation_.x, worldTransform3Dreticle_.translation_.y, worldTransform3Dreticle_.translation_.z);
	ImGui::End();
#endif // _DEBUG

}

