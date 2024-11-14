#pragma once
#include "WorldTransform.h"
#include <Model.h>

class Enemy;

/// <summary>
/// 敵の攻撃
/// </summary>
class EnemyCanon {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Enemy* enemy, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// デスフラグゲッター
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	Enemy* enemy_ = nullptr;

	uint32_t textureHandel_;

	// 速度
	Vector3 velocity_ = {};

	// 寿命<frame>
	static const int32_t kLifeTime_ = 60 * 3;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime_;
	// デスフラグ
	bool isDead_ = false;


};
