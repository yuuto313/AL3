#include "Enemy.h"
#include <cassert>
#include "TextureManager.h"
#include "Player.h"
#include "GameScene.h"
Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model,const Vector3& approachVelocity, const Vector3& leaveVelocity) { 
	assert(model);
	// テクスチャを読み込む
	textureHandle_ = TextureManager::Load("enemy.png"); 
	model_ = model;
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;
	
	//接近フェーズ初期化
	InitializeApploach();

}

void Enemy::Update() { 
	//--------------------------------
	// 行動フェーズ
	//--------------------------------
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

	//--------------------------------
	// 行列の更新
	//--------------------------------
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::InitializeApploach() {
    //発射タイマー初期化
	shotTimer_ = kFireInterval;
}

void Enemy::UpdateApploach() {
	// 移動（ベクトル加算）
	worldTransform_.translation_ += approachVelocity_;
	// 基底の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	//発射タイマーカウントダウン
	--shotTimer_;
	if (shotTimer_ <= 0) {
	    //弾を発射
		Fire();
		//発射タイマーを初期化
		shotTimer_ = kFireInterval;
	}

}

void Enemy::UpdateLeave() {
	// 移動（ベクトル加算）
	worldTransform_.translation_ += leaveVelocity_;
}

void Enemy::Fire() {
	assert(player_);

	//弾の速さ（調整項目）
	const float kBulletSpeed = -1.0f;
	
	//自キャラのワールド座標を取得
	Vector3 playerWorldPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得
	Vector3 enemyWorldPos = GetWorldPosition();
	//敵キャラから自キャラへの差分ベクトルを求める
	Vector3 diff = enemyWorldPos - playerWorldPos;
	//ベクトルの正規化
	diff = Normalize(diff);
	
	diff = diff * kBulletSpeed;

	Vector3 velocity = diff;

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_,velocity);
	// ゲームシーンの敵弾リストに弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {
	// 何もしない
}
