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
	virtual void OnCollison() {}
	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetCeterPosition() const = 0;
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

private:
	//衝突判定
	float radius_ = 1.5f;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
};
