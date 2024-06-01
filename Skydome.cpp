#include "Skydome.h"
#include <cassert>

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	// nullptrチェック
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.f, 1.f, 1.f};
}

void Skydome::Update() {
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
