#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include "DebugCamera.h"

#pragma once
/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:

	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model,ViewProjection* viewProjection);
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
	//モデル
	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;
};
