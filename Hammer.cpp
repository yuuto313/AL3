#include "Hammer.h"

void Hammer::Initialize(Model* model, Player* player) {}

void Hammer::Update() {}

void Hammer::Draw() {}

void Hammer::BehaviorAttackUpdate() {
	if (currentRotationAngleX < targetRotationAngleX) {
		currentRotationAngleX += rotationSpeed;
		if (currentRotationAngleX > targetRotationAngleX) {
			currentRotationAngleX = targetRotationAngleX;
		}
	} else {
		behaviorRequest_ = Behavior::kRoot;
	}

	worldTransform_.rotation_.x = currentRotationAngleX;
}

void Hammer::OnCollision() {}
