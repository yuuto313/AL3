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
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
