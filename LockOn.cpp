#include "LockOn.h"

void LockOn::Initalize() {
	//スプライトの生成
	lockOnMark_.reset(Sprite::Create(textureHandle_, {}, {1.0f,1.0f,1.0f,1.0f},))
}

void LockOn::Update() {}

void LockOn::Draw() {}
