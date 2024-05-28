#include "EnemyBullet.h"
#include <cassert>
#include "TextureManager.h"
EnemyBullet::EnemyBullet() {}

EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) { 
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("bullet.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void EnemyBullet::Update() { 
	//--------------------------------
	// 時間経過でデス
	//--------------------------------
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;


	//--------------------------------
	// 行列の更新
	//--------------------------------
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
