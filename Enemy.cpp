#include "Enemy.h"
#include "CollisionTypeIdDef.h"

uint32_t Enemy::nextSerialNumber_ = 0;

Enemy::Enemy() {
	//シリアル番号を振る
	serialNumber_ = nextSerialNumber_;
	//次のシリアルナンバー番号を1加算
	++nextSerialNumber_;
}

void Enemy::Initialize(const std::vector<Model*>& models) { 
	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	//ワールド行列の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 3.0f};

	worldTransformLeftWeapon_.Initialize();
	worldTransformLeftWeapon_.translation_ = {3.0f, 1.8f, 0.0f};
	worldTransformRightWeapon_.Initialize();
	worldTransformRightWeapon_.translation_ = {-3.0f, 1.8f, 0.0f};

	//本体と親子関係を結ぶ
	worldTransformLeftWeapon_.parent_ = &worldTransform_;
	worldTransformRightWeapon_.parent_ = &worldTransform_;

	//識別IDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::KEnemy));
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
	worldTransformLeftWeapon_.UpdateMatrix();
	worldTransformRightWeapon_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransform_, viewProjection);
	models_[1]->Draw(worldTransformLeftWeapon_, viewProjection);
	models_[2]->Draw(worldTransformRightWeapon_, viewProjection);
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

	//--------------------------------
	// 武器にアニメーションを付ける
	//--------------------------------
	
	// 左右の武器を回転させる
	const float rotationX = 0.05f;
	worldTransformLeftWeapon_.rotation_.x += rotationX;
	worldTransformRightWeapon_.rotation_.x += rotationX;
}

Vector3 Enemy::GetCenterPosition()const {
	//見た目上の中心点オフセット
	const Vector3 offset = {0.0f, 0.0f, 0.0f};
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;	
}

void Enemy::OnCollision(Collider* other) {
	// 衝突相手の識別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon)) {
		// デスフラグを立てる
		isDead_ = true;
	}
}
