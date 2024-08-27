#include "Hammer.h"
#include <cassert>

void Hammer::Initialize(Model* model, Player* player) { 
	assert(model);
	model_ = model;
	player_ = player;
	worldTransform_.Initialize();
}

void Hammer::Update() { 
	worldTransform_.UpdateMatrix();
}

void Hammer::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);
}

void Hammer::BehaviorAttackUpdate() {}

void Hammer::OnCollision() {}
