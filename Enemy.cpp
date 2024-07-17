#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models) { 
	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	//ワールド行列の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 3.0f};
	worldTransformWeapon_.Initialize();

	//本体と親子関係を結ぶ
	worldTransformWeapon_.parent_ = &worldTransform_;

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
	worldTransformWeapon_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransform_, viewProjection);
	models_[1]->Draw(worldTransformWeapon_, viewProjection);
}

void Enemy::Movement() { 
	//--------------------------------
	//  回転処理
	//--------------------------------
	//回転速度
	Vector3 rotationSpeed = {0.0f, 0.03f, 0.0f};
	
	//速度ベクトルを自機の向きに合わせて回転させる
	rotationSpeed = TransformNormal(rotationSpeed, worldTransform_.matWorld_);

	worldTransform_.rotation_ += rotationSpeed;

	//--------------------------------
	// 回転角度から位置を決める
	//--------------------------------
	//円の半径
	const float radius = 15.0f;

	worldTransform_.translation_.x = radius * cos(worldTransform_.rotation_.y);
	worldTransform_.translation_.z = radius * sin(worldTransform_.rotation_.y);
}

Vector3 Enemy::GetCenterPosition()const {
	//見た目上の中心点オフセット
	const Vector3 offset = {0.0f, 1.0f, 0.0f};
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;	
}
