#pragma once
#include "Collider.h"
#include <list>
#include <vector>
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "Player.h"
#include "Enemy.h"

class PlayerBullet;
class EnemyBullet;

class CollisionManager {
public:

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions(Player* player,Enemy* enemy);

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private:
	//コライダーリスト
	std::list<Collider*> colliders_;
};
