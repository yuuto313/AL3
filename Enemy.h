#include "Model.h"
#include "WorldTransform.h"
#pragma once

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& approachVelocity,const Vector3& leaveVelocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);
	/// <summary>
	/// 接近フェーズの更新関数
	/// </summary>
	void UpdateApploach();
	/// <summary>
	/// 離脱フェーズの更新関数
	/// </summary>
	void UpdateLeave();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルのポインタ
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 approachVelocity_;
	Vector3 leaveVelocity_;

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;
};
