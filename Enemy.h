#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"

class Player;

class Enemy {
public:
	Enemy();
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

	//歩行の速さ
	static inline const float kWalkSpeed = 0.05f;
	//速度
	Vector3 velocity_ = {};

	//最初の角度（度）
	static inline const float kWalkMotionAngleStart =  -10.0f;
	//最後の角度（度）
	static inline const float kWalkMotionAngleEnd = 40.0f;
	//アニメーションの周期となる時間（秒）
	static inline const float kWalkMotionTime = 3.0f;
	//経過時間
	float walkTimer_ = 0.0f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

public:
	/// <summary>
	/// ワールド座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// AABBを取得する
	/// </summary>
	/// <returns></returns>
	AABB GetAABB();

	/// <summary>
	/// 衝突応答
	/// </summary>
	void OnCollision(const Player* player);
};
