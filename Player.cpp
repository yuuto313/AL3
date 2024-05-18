#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <algorithm>
#include "DebugText.h"
#include "MapChipField.h"


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

	// 1.移動入力
	MovementInput();

	// 2.移動量を加味して衝突判定する
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.amountOfMovement = velocity_;

	// マップ衝突チェック
	IsCollision(collisionMapInfo);

	// 3.判定結果を反映して移動させる
	Reflection(collisionMapInfo);

	// 4.天井に当たってる場合の処理
	HitsTheCeiling(collisionMapInfo);

	// 5.
	
    // 6.設置状態の切り替え
	InstallationStatus(collisionMapInfo);

	// 7.旋回制御
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
		worldTransform_.rotation_.y += (destinationRotationY - worldTransform_.rotation_.y) * t;
		
		
	}

	// 8.行列計算
	worldTransform_.UpdateMatrix();

}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); 
}

void Player::MovementInput() {

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

	// 移動入力
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
		// 左右移動操作
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

    // 空中
	} else {
		// 落下速度
		velocity_ += Vector3(0.f, -kGravityAcceleration, 0.f);
		// 落下速度制限
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

	// 移動
	worldTransform_.translation_ += velocity_;
}

void Player::IsCollision(CollisionMapInfo& info) { 
	IsCollisionUp(info);
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}  //  kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::IsCollisionUp(CollisionMapInfo& info) {
	//上昇あり？
	if (info.amountOfMovement.y <= 0) {
		return;
	}
	
	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionNew{};

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.amountOfMovement, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//真上の当たり判定を行う
	bool hit = false;
	//左上点の当たり判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//右上点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//ブロックにヒット
	if (hit) {
	    //めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + positionNew[kRightBottom]);
		//めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		//Y移動量を求める
		//float yMovement = (rect.bottom - worldTransform_.translation_.y) - ((kHeight / 2) + kBlank);
		info.amountOfMovement.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - ((kHeight / 2) + kBlank));

		//天井に当たったことを記録する
		info.ceilingCollision = true;
	}
}

void Player::IsCollisionDown(CollisionMapInfo& info) {
	//降下あり？
	if (info.amountOfMovement.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionNew{};

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.amountOfMovement, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//真下の当たり判定を行う
	bool hit = false;
	//左下点の当たり判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	//右下点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + positionNew[kRightBottom]);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		// Y移動量を求める
		float yMovement = (rect.top - worldTransform_.translation_.y) + ((kHeight / 2) + kBlank);
		info.amountOfMovement.y = std::max(0.0f, yMovement);

		// 地面に当たったことを記録する
		info.ceilingCollision = true;
	}
}

void Player::InstallationStatus(const CollisionMapInfo& info) {
	if (onGround_) {
	//設置状態の処理
		//ジャンプ開始
		if (velocity_.y > 0.0f) {
		onGround_ = false;
		} else {

			// 移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionNew{};

			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.amountOfMovement, static_cast<Corner>(i));
			}

			//落下判定
			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool hit = false;
			// 左下点の当たり判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0.0f, -kAdjustment, 0.0f));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下点の当たり判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0.0f, -kAdjustment, 0.0f));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			//落下開始
			if (!hit) {
			//落下なら空中状態に切り替える
				onGround_ = false;
			}
		}

	} else {
	//空中状態の処理
		if (info.landing) {
		//着地状態に切り替える（落下を止める）
			onGround_ = true;
			//着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;

		}
	}
}

void Player::Reflection(const CollisionMapInfo& info) { 
	worldTransform_.translation_ += info.amountOfMovement; }

void Player::HitsTheCeiling(const CollisionMapInfo& info) {
	if (info.ceilingCollision) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

