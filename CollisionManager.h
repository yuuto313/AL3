#pragma once
#include <list>
#include "Collider.h"
#include "Model.h"
#include "GlobalVariables.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// ワールドトランスフォームの更新
	/// </summary>
	void UpdateWorldTransform();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	/// <summary>
	/// すべての当たり判定チェック
	/// </summary>
	void CheckAllCollisons();
	/// <summary>
	/// リストに登録
	/// </summary>
	void AddCollider(Collider* collider);

private:
	//コライダー
	std::list<Collider*> colliders_;
	//デバッグ表示用モデル
	std::unique_ptr<Model> model_;

	bool drawCollider_ = false;
};
