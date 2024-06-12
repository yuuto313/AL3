#include "Skydome.h"
#include "ViewProjection.h"

void Skydome::Initialize() {}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
