#include "Enemy.h"
#include <cassert>
#include "TextureManager.h"

Enemy::Enemy() { 
	state_ = new EnemyStateApproach(); 
}

Enemy::~Enemy() { 
	delete state_;
}


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

void Enemy::UpdateApproach() {
	// 移動（ベクトル加算）
	worldTransform_.translation_ += approachVelocity_;
	// 基底の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave() {
	// 移動（ベクトル加算）
	worldTransform_.translation_ += leaveVelocity_;
}

Vector3 Enemy::UpdateTranslation(Vector3& position, const Vector3& velocity) {
	position += velocity; 
	return position;
}

void Enemy::ChangeState(BaseEnemyState* newState) {
	delete state_; 
	state_ = newState;
}

void EnemyStateApproach::Update() { 
	Vector3 pos = enemy_->GetPosition();
	Vector3 approachVelocity = enemy_->GetApproachVelocity();
	enemy_->UpdateTranslation(pos, approachVelocity);
	if (pos.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave);
	}
}

void EnemyStateLeave::Update() {
	Vector3 pos = enemy_->GetPosition();
	Vector3 leaveVelocity = enemy_->GetLeaveVelocity();
	pos += leaveVelocity;
}
