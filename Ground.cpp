#include "Ground.h"
#include "cassert"
void Ground::Initialize(Model* model, ViewProjection* viewProjection) {
	// nullptrチェック
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.f, 1.f, 1.f};
}

void Ground::Update() {}

void Ground::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); 
}
