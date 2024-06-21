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

	//Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	// Y軸周りの回転角度(θy)を計算
	// atan2: 第一引数に高さ、第二引数に底辺を指定する
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	Vector3 newVelocity = velocity_;
	newVelocity.y = 0;
	float velocityXZ = Length(newVelocity);

	// X軸周りの回転角度(θx)を計算
	worldTransform_.rotation_.x = std::atan2(-velocity_.y,velocityXZ );
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
