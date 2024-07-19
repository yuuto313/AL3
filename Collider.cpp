#include "Collider.h"

void Collider::Initialize() {
	worldTransform_.Initialize();
	worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};
}

void Collider::UpdateWorldTransform() {
	//ワールド座標をワールドトランスフォームに適用
	worldTransform_.translation_ = GetCeterPosition();
	worldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) { 
	model->Draw(worldTransform_, viewProjection);
}
