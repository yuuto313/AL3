#include "Skydome.h"

Skydome::Skydome() {
}

Skydome::~Skydome() {}

void Skydome::Init(Model* model, ViewProjection* viewProjection) { 
	//nullptrチェック
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
}

void Skydome::Update() {
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}