#include "CollisionManager.h"

void CollisionManager::CheckAllCollisions() {
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerbullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemybullets = enemy_->GetBullets();

	// コライダーをリストに登録
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);
	// 自弾全てについて
	for (PlayerBullet* playerBullet : playerbullets) {
		colliders_.push_back(playerBullet);
	}
	// 敵弾全てについて
	for (EnemyBullet* enemyBullet : enemybullets) {
		colliders_.push_back(enemyBullet);
	}

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素からまわす（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}
