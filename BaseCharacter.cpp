#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) {
	Collider::Initialize();
	models_ = models;
	worldTransform_.Initialize();
}

void BaseCharacter::Update() { 
	worldTransform_.UpdateMatrix();
}

void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	//モデル描画
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

Vector3 BaseCharacter::GetCeterPosition() const {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
