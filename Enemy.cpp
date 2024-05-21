#include "Enemy.h"
#include <numbers>
#include <cassert>
#include <cmath>

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position) { 
	assert(model);
	//モデルをメンバ変数に記録
	model_ = model;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//初期座標の設定
	worldTransform_.translation_ = position;
	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed, 0.f, 0.f};

	// 初期回転角を指定する
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	walkTimer_ = 0.0f;

}

void Enemy::Update() { 
	//移動
	worldTransform_.translation_ += velocity_;

	//タイマーを加算
	walkTimer_ += 1.0f / 60.f;

	//回転アニメーション
	float param = std::sin(2 * static_cast<float>(M_PI) * walkTimer_);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = Radians(radian);

	//座標を基に行列の計算を行う
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_);
}
