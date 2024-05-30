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

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変数の初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

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
	// 行列を定数バッファに転送する
	worldTransform_.TransferMatrix();
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
	if (input_->TriggerKey(DIK_W)) {
	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	//弾を生成し、初期化
	PlayerBullet* newBullet = new PlayerBullet();
	newBullet->Initialize(model_, worldTransform_.translation_,velocity);
	//弾を登録する
	bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {
    //ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void Player::OnCollision() {
//何もしない
}

