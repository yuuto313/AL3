#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
/// <summary>
/// 衝突判定オブジェクト	
/// </summary>
class Collider {
public:
	virtual ~Collider() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	void Draw(Model* model,const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision([[maybe_unused]]Collider* other) {}

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetCenterPosition() const = 0;

	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="radius"></param>
	/// <returns></returns>
	float SetRadius(float radius) { radius = radius_; }

	/// <summary>
	/// 識別IDを取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetTypeID() const { return typeID_; }

	/// <summary>
	/// 識別IDを設定
	/// </summary>
	/// <param name="typeID"></param>
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; } 

	private:
	//衝突判定
	float radius_ = 1.5f;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//識別ID
	uint32_t typeID_ = 0u;
};
