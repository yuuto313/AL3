#pragma once
#include <array>

#include "Model.h" 
#include "ViewProjection.h"
#include "Player.h"

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles {
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	/// <param name="player"></param>
	void Initialize(Model* model,ViewProjection* viewProjection,const Player* player,const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	Model* model_ = nullptr;
	ViewProjection* viewProjevtion_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransforms_;



};
