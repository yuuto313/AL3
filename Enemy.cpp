#include "Enemy.h"
#include <cassert>
#include "TextureManager.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	// bullet_の解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

	//timedCall_の解放
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
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
	
	//接近フェーズ初期化
	InitializeApploach();

}

void Enemy::Update() { 
	//--------------------------------
	// デスフラグの立った弾を削除
	//--------------------------------

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//--------------------------------
	// 攻撃処理
	//--------------------------------

	// 終了したタイマーを削除する
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			delete timedCall;
			return true;
		}
		return false;
	});


	// 範囲for文でリストの全要素についてまわす
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

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
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::InitializeApploach() {
	//発射タイマーをセットする
	ResetTimer();

}

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

	//登録済みの時限発射イベントリストをクリアする
	timedCalls_.clear();
}

void Enemy::Fire() {
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_,velocity);
	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::ResetTimer() {
	//弾を発射する
	Fire();

	//発射間隔
	uint32_t interval = 60;

	//発射タイマーをリセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::ResetTimer, this), interval));
}
