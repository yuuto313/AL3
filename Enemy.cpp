#include "Enemy.h"
#include <cassert>
#include "TextureManager.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}


void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& approachVelocity, const Vector3& leaveVelocity) { 
	assert(model);
	// テクスチャを読み込む
	textureHandle_ = TextureManager::Load("enemy.png"); 
	model_ = model;
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;

	//初期状態をセット
	ChangeState(std::make_unique<EnemyStateApproach>(this));
}

void Enemy::Update() { 
	//遷移変更
	state_->Update();

	// 行列を更新
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); }

Vector3 Enemy::GetPosition() {// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::UpdateTranslation(const Vector3& velocity) {
	worldTransform_.translation_ += velocity; 
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> newState) { 
	state_ = std::move(newState);
}

void EnemyStateApproach::Update() { 
	Vector3 approachVelocity = enemy_->GetApproachVelocity();
	Vector3 pos = enemy_->GetPosition();
	enemy_->UpdateTranslation(approachVelocity);
	if (pos.z < 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
}

void EnemyStateLeave::Update() {
	Vector3 leaveVelocity = enemy_->GetLeaveVelocity();
	enemy_->UpdateTranslation(leaveVelocity);
}
