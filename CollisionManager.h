#pragma once
#include "Collider.h"
#include <list>
#include <vector>
class CollisionManager {
public:

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

private:
	//コライダーリスト
	std::list<Collider*> colliders_;

};
