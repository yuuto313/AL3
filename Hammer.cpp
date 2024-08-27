#include "Hammer.h"
#include "Player.h"
#include "CollisionTypeIdDef.h"
#include <cassert>

void Hammer::Initialize(Model* model, Player* player) { 
	Collider::Initialize();

	assert(model);
	model_ = model;
	player_ = player;
	worldTransform_.Initialize();

	// 識別IDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));
}

void Hammer::Update() { 
	worldTransform_.UpdateMatrix();
}

void Hammer::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);
}

void Hammer::BehaviorAttackInitialize() {
	// ギミックのアニメーション用数値のリセット
	currentRotationAngleX = 0.0f;
}

void Hammer::BehaviorAttackUpdate() {

	//// ロックオン中
	//if (lockOn_ && lockOn_->ExistTarget()) {
	//	// ロックオン座標
	//	Vector3 lockOnPosition = lockOn_->GetTargetPosition();
	//	// 追従対象からロックオン対象へのベクトル
	//	Vector3 sub = lockOnPosition - worldTransform_.translation_;

	//	// 距離
	//	float distance = Length(sub);
	//	// 距離しきい値
	//	const float threshold = 0.2f;

	//	// しきい値より離れているときのみ
	//	if (distance > threshold) {
	//		// Y軸周り角度
	//		worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
	//		// 追い越し防止処理未実装
	//	}
	//}

	if (currentRotationAngleX < targetRotationAngleX) {
		currentRotationAngleX += rotationSpeed;
		if (currentRotationAngleX > targetRotationAngleX) {
			currentRotationAngleX = targetRotationAngleX;
		}
	} else {
		player_->SetBehavior(Behavior::kRoot);
	}

	worldTransform_.rotation_.x = currentRotationAngleX;

}

Vector3 Hammer::GetCenterPosition() const { 
	//ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 10.0f, 0.3f};
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Hammer::OnCollision([[maybe_unused]] Collider* other) {
	//衝突相手の識別IDを取得
	uint32_t typeID = other->GetTypeID();

	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::KEnemy)) {
		enemy_ = static_cast<Enemy*>(other);
		//敵の位置にエフェクト発生
		enemy_->Reaction();
	}
}
