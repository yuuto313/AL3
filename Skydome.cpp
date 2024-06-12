#include "Skydome.h"
#include "ViewProjection.h"
#include "cassert"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	// nullptrチェック
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.f, 1.f, 1.f};
}

void Skydome::Update() {}

void Skydome::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}
