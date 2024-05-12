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
	void Init(Model* model,ViewProjection* viewProjection,const Vector3& position);
	void Update();
	void Draw();

	

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};
	
	//加速度
	static inline const float kAcceleration = 0.01f;
	//速度減衰率
	static inline const float kAttenuation = 0.06f;
	//速度制限
	static inline const float kLimitRunSpeed = 0.1f;

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	//プレイヤーの向き	
	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;

	//旋回時間＜秒＞
	static inline const float kTimeTurn = 2.f;

	//設置状態フラグ
	bool onGround_ = true;

	//重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.3f;
	//最大重力加速度（下方向）
	static inline const float kLimitFallSpeed = 3.0f;
	//ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 1.0f;

};
