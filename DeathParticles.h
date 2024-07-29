#pragma once
#include <array>

#include "Model.h" 
#include "ViewProjection.h"
#include "WorldTransform.h"

//フェードアウトから！

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	/// <param name="player"></param>
	void Initialize(Model* model,ViewProjection* viewProjection,const Vector3& position);

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

	//存続時間（消滅までの時間）＜秒＞
	static inline const float kDuration = 2.0f;
	//移動の速さ
	static inline const float kSpeed = 0.08f;
	//分割数
	static inline const uint32_t kDivision = 8;
	//分割した1個分の角度
	static inline const float kAngleUnit = 2.0f * (float)M_PI / static_cast<float>(kDivision);


	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

};
