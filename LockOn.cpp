#include "LockOn.h"
#include "TextureManager.h"

void LockOn::Initalize() {
	textureHandle_ = TextureManager::Load("lockOn.png");
	//スプライトの生成
	lockOnMark_.reset(Sprite::Create(textureHandle_, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {
	
}

void LockOn::Draw() { 
	//ロックオン中なら
	if (target_) {
		lockOnMark_->Draw();
	}
}
