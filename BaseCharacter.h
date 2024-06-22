#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "vector"

/// <summary>
/// キャラクター基底クラス
/// </summary>
class BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	virtual void Initialize(const std::vector<Model*>& models);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);
	/// <summary>
	/// ワールド変換データ
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

protected:
	//モデルデータ配列
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
};
