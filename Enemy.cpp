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
}

void Enemy::Update() { 
	switch (phase_) {
	case Enemy::Phase::Approach:
		//移動（ベクトル加算）
		UpdateApploach();
		break;
	case Enemy::Phase::Leave:
		// 移動（ベクトル加算）
		UpdateLeave();
		break;
	default:
		break;
	}
	// 行列を更新
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void Enemy::UpdateApploach() {
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
