#include "CollisionManager.h"
#include "MyMath.h"
void CollisionManager::Reset() {
	//リストを空っぽにする
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA = colliderA->GetCeterPosition();
	Vector3 posB = colliderB->GetCeterPosition();
	//座標の差分ベクトル
	Vector3 subtract = posB - posA;
	//座標AとBの距離を求める
	float distance = Length(subtract);
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollison();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollison();
	}

}

void CollisionManager::CheckAllCollisons() {
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//イテレーターBはイテレーターAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
	
}

void CollisionManager::AddCollider(Collider* collider) {
	if (collider != nullptr) {
		colliders_.push_back(collider);
	}
}
