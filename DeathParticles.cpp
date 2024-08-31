#include "DeathParticles.h"
#include <cassert>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, Player* player, const Vector3& position) {
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {
	//ワールド変換の更新
	for (auto& worldtransform : worldTransforms_) {
		worldtransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	//モデルの描画
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_);
	}
}
