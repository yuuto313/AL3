#include "Skydome.h"

void Skydome::Initialize() {}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
}
