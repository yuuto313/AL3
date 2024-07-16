#pragma once
#include <list>
#include "Collider.h"
/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:
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
};
