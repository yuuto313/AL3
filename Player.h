#include "Model.h"
#include "WorldTransform.h"
#pragma once
/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	void Init();
	void Update();
	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
