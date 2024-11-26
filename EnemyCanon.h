#pragma once
#include "WorldTransform.h"
#include "Collider.h"
#include <Model.h>

class Enemy;
class Player;

/// <summary>
/// 敵の攻撃
/// </summary>
class EnemyCanon final : public Collider{
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

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 親子関係を結ぶ
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 衝突を検出したら呼び出される
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const override;

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	Enemy* enemy_ = nullptr;
	Player* player_ = nullptr;

	// 速度
	Vector3 velocity_ = {};

	// 寿命<frame>
	static const int32_t kLifeTime_ = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime_;
	// デスフラグ
	bool isDead_ = false;

	uint32_t damage_ = 50;

};
