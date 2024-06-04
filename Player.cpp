#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"
Player::Player() {}

Player::~Player() {
	// bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, uint32_t reticleTextureHandle,const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	reticleTextureHandle_ = reticleTextureHandle;
	// ワールド変数の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3Dreticle_.Initialize();
}


void Player::Update() {
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
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	const float kcharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更する（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kcharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kcharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更する（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kcharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kcharacterSpeed;
	}
	// 座標移動(ベクトルの加算）
	worldTransform_.translation_ += move;

	//--------------------------------
	//移動制限
	//--------------------------------
	//移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 17;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	//--------------------------------
	//旋回
	//--------------------------------

	Rotate();

	//--------------------------------
	//攻撃処理
	//--------------------------------

	Attack();

	//弾を更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//--------------------------------
	//行列の更新
	//--------------------------------
	worldTransform_.UpdateMatrix();

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
		worldTransform3Dreticle_.translation_ = offset;
		worldTransform3Dreticle_.UpdateMatrix();
	}

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
	
	//レティクル
	model_->Draw(worldTransform3Dreticle_, viewProjection);

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
	if (input_->TriggerKey(DIK_W)) {
	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

	//自機から標準オブジェクトへのベクトル
	velocity = worldTransform3Dreticle_.translation_ - worldTransform_.translation_;
	velocity = Normalize(velocity)*kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	//弾を生成し、初期化
	PlayerBullet* newBullet = new PlayerBullet();
	newBullet->Initialize(model_, GetWorldPosition(), velocity);
	//弾を登録する
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

void Player::OnCollision() {
//何もしない
}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

