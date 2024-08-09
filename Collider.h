#pragma once
#include <Vector3.h>
#include <cstdint>

class Collider {
public:
	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision() {}

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	virtual Vector3 GetWorldPosition() const = 0;

	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float radius) { radius_ = radius; }

	/// <summary>
	/// 衝突判定（自分）を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }

	/// <summary>
	/// 衝突判定（自分）を設定
	/// </summary>
	/// <param name="collisionAtrribute"></param>
	void SetCollisionAttribute(uint32_t collisionAtrribute) { collisionAttribute_ = collisionAtrribute; }

	/// <summary>
	/// 衝突判定（相手）を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionMask() { return collisionMask_; }

	/// <summary>
	/// 衝突判定（相手）を設定
	/// </summary>
	/// <param name="collisonMask"></param>
	void SetCollisionMask(uint32_t collisonMask) { collisionMask_ = collisonMask; }

private:
	float radius_ = 1.0f;

	//衝突属性（自分）
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク（相手）
	uint32_t collisionMask_ = 0xffffffff;


};
