#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <algorithm>

float easeInOutSine(float t) { return - (cosf(static_cast<float>(M_PI)) * t) - 1.0f / 2.0f;}

Player::Player() {}

Player::~Player() {}

void Player::Init(Model* model, ViewProjection* viewProjection,const Vector3& position) { 
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	//ワールド変換初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//初期回転角を指定する
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

}

void Player::Update() {
	// ワールド行列の転送
	worldTransform_.TransferMatrix();

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 降下中？
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {

			// 空中状態に移行
			onGround_ = false;
		}
		
		// 移動入力
		// // 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {

					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				if (lrDirection_ != LRDirection::kRight) {

					lrDirection_ = LRDirection::kRight;

					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
				acceleration.x += kAcceleration;

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {

					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				if (lrDirection_ != LRDirection::kLeft) {

					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
				acceleration.x -= kAcceleration;
			}

			// 加速/減速
			velocity_ += acceleration;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		
		} else {

				// 非入力時は移動減衰をかける
				velocity_.x *= (1.0f - kAttenuation);
			}

		if (Input::GetInstance()->PushKey(DIK_UP)) {

			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}

		//空中
	} else {
		//落下速度
		velocity_ += Vector3(0.f, -kGravityAcceleration, 0.f);
		//落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	
		// 着地
		if (landing) {

			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度減衰
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 設置状態に移行
			onGround_ = true;
		}

	}

	// 旋回制御
	if (turnTimer_ > 0.0f)
	{
		//旋回タイマーを1/60秒分カウントダウンする	
		turnTimer_ -= 1.f / 60.f;

		// 補間係数を計算する
		float t = 1.0f - (turnTimer_ / kTimeTurn);

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> * 3.0f / 2.0f
		};

		// 状態の応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		//worldTransform_.rotation_.y = easeInOutSine(t);
		worldTransform_.rotation_.y += (destinationRotationY - worldTransform_.rotation_.y) * t;
		;
		//worldTransform_.rotation_.y = destinationRotationY;
		
	}

	//移動
	worldTransform_.translation_ += velocity_;

	//行列計算
	worldTransform_.UpdateMatrix();

}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); 
}

