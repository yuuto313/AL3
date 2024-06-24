#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models) { 
	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	//ワールド行列の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 10.0f};
}

void Enemy::Update() {
	//--------------------------------
	// 基底クラスの更新
	//--------------------------------

	BaseCharacter::Update();

	//--------------------------------
	// 移動処理
	//--------------------------------

	Movement();

	//--------------------------------
	// ワールド行列の更新
	//--------------------------------

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransform_, viewProjection);
}

void Enemy::Movement() { 
	//const float speed = 1.0f;

	Vector3 velocity = {0.0f, 0.3f, 0.0f};

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	worldTransform_.rotation_ += velocity;

}
