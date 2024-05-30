#include "WorldTransform.h"
#include "Model.h"

#pragma once
/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	Skydome();
	~Skydome();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	//モデル
	Model* model_ = nullptr;
};
