#pragma once
#include "Model.h"
#include "ViewProjection.h"

class Player;

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//モデルのポインタ
	Model* model_ = nullptr;

	//ビュープロジェクションのポインタ
	ViewProjection viewProjection_;
};
