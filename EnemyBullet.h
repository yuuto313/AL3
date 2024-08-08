#include "WorldTransform.h"
#include "Model.h"
#include "Collider.h"

#pragma once
/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet : public Collider{
public:
	EnemyBullet();
	~EnemyBullet();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// isDead_のゲッター
	/// </summary>
	/// <returns></returns>
	/// 
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;
	
	/// <summary>
	/// 半径を取得するゲッター
	/// </summary>
	/// <param name="radius"></param>
	/// <returns></returns>
	float GetRadius() { return kRadius; }


private:
	// モデルのポインタ
    Model* model_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	//半径
	static inline float kRadius = 1.0f;
};
