#pragma once
class Collider {
public:
	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	virtual Vector3 GetWorldPosition() = 0;

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

private:
	float radius_ = 1.0f;
};
