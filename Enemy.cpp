#include "Enemy.h"
#include "CollisionTypeIdDef.h"
#include "Player.h"
#include "LockOnPlayer.h"

uint32_t Enemy::nextSerialNumber_ = 0;

Enemy::Enemy() {
	//シリアル番号を振る
	serialNumber_ = nextSerialNumber_;
	//次のシリアルナンバー番号を1加算
	++nextSerialNumber_;
}

Enemy::~Enemy() {
	for (EnemyCanon* canon : canons_) {
		delete canon;
	}

	canons_.clear();

}

void Enemy::Initialize(const std::vector<Model*>& models) { 
	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	//ワールド行列の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 3.0f};
	worldTransformWeapon_.Initialize();

	//本体と親子関係を結ぶ
	worldTransformWeapon_.parent_ = &worldTransform_;

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

	//Movement();

	//--------------------------------
	// 攻撃処理
	//--------------------------------

	Attack();

	for (EnemyCanon* canon_ : canons_) {
		canon_->Update();
	}

	//--------------------------------
	// ワールド行列の更新
	//--------------------------------

	worldTransform_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransform_, viewProjection);

	for (EnemyCanon* canon : canons_) {
		canon->Draw(viewProjection);
	}

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

void Enemy::Attack() {
	
	// デスフラグが立った大砲を削除
	canons_.remove_if([](EnemyCanon* canon) {
		if (canon->IsDead()) {
			delete canon;
			return true;
		}
		return false;
	});

	// 球の速度
	const float kBulletSpeed = -1.0f;
	// 敵キャラの座標を取得
	Vector3 translate = GetWorldPosition();
	// 敵キャラの座標を取得
	Vector3 playerTranslate = player_->GetWorldPosition();
	// 敵キャラから自キャラへの差分ベクトルを求める
	Vector3 diff = translate - playerTranslate;
	// ベクトルの正規化
	diff = Normalize(diff);
	// ベクトルの長さを速さに合わせる
	diff = diff * kBulletSpeed;

	Vector3 velocity = diff;

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	
	// 3秒間隔で大砲を生成する
	float deltaTime = 1.0f / 60.0f;

	coolTime_ -= deltaTime;

	if (coolTime_ <= 0.0f) {
		EnemyCanon* newCanon = new EnemyCanon();
		newCanon->Initialize(models_[1], this, velocity);
		newCanon->SetPlayer(player_);

		// 弾を登録する
		canons_.push_back(newCanon);

		coolTime_ = 3.0f;
	}

}

void Enemy::Reaction() {
	Vector3 scale = Collider::GetScale();
	Vector3 baseScale = {2.0f, 2.0f, 2.0f};
	float increment = 0.1f;

	// フレームごとの時間差
	float deltaTime = 1.0f / 60.f;
	// 時間経過を追跡する変数
	float elapsedTIme = 0.0f;

	elapsedTIme += deltaTime;

	if (elapsedTIme <= 1.0f) {
		scale.x += increment;
		scale.y += increment;
		scale.z += increment;
		Collider::SetScale(scale);
	} else {
		elapsedTIme = 0.0f;
	}
}


Vector3 Enemy::GetCenterPosition()const {
	//見た目上の中心点オフセット
	const Vector3 offset = {0.0f, 0.0f, 0.0f};
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;	
}

Vector3 Enemy::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
