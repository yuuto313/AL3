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
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void EnemyBullet::OnCollision() {
	// デスフラグを立てる
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}
