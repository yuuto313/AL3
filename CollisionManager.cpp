#include "CollisionManager.h"
#include "MyMath.h"

void CollisionManager::CheckAllCollisions(Player* player, Enemy* enemy) {

	//衝突マネージャのリストをクリアする
	colliders_.clear();

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerbullets = player->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemybullets = enemy->GetBullets();

	// コライダーをリストに登録
	colliders_.push_back(player);
	colliders_.push_back(enemy);

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

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突フィルタリング
	if (colliderA->GetCollisionAttribute() != colliderB->GetCollisionMask() || colliderB->GetCollisionAttribute() != colliderA->GetCollisionMask()) {
		return;
	}

	// コライダーAとBのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	Vector3 difference = posB - posA;

	float distance = Length(difference);

	// 球と球の当たり判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// コライダーAとBの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

