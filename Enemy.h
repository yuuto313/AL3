#include "Model.h"
#include "WorldTransform.h"
#include "BaseEnemyState.h"
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
	/// 座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition();
	/// <summary>
	/// 接近中の速度のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetApproachVelocity() const { return approachVelocity_; }
	/// <summary>
	/// 離脱中の速度のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetLeaveVelocity() const { return leaveVelocity_; }
	/// <summary>
	/// 引数で指定した移動量だけ座標を変更する
	/// </summary>
	/// <param name="velocity"></param>
	void UpdateTranslation(const Vector3& velocity);
	/// <summary>
	/// ステータスを変更する
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(BaseEnemyState* newState);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルのポインタ
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 approachVelocity_ = {0,0,0};
	Vector3 leaveVelocity_ = {0,0,0};

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;

	BaseEnemyState* state_ = nullptr;
};
