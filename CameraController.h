#include "ViewProjection.h"
#include "WorldTransform.h"


//前方宣言
class Player;

#pragma once
/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {
public:
	CameraController();
	~CameraController();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* viewProjection_);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	ViewProjection* viewProjection_ = nullptr;

	//追従対象となるプレイヤーから継続的に座標を取得するためにプレイヤーのポインタを持たせる
	Player* target_ = nullptr;

	//追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0.f, 0.f, -70.0f};

	//矩形
	struct Rect {
		float left = 0.0f;//左端
		float right = 1.0f;//右端
		float bottom = 0.0f;//下端
		float top = 1.0f;//上端
	};

	//カメラの移動範囲
	Rect movableArea_ = {0.f, 100.f, 0.f, 100.f};
	
	//カメラの目標座標
	Vector3 targetPosition_ = {};

	//座標補間割合
	static inline const float kInterpolationRate = 2.5f;

	// 速度掛け率
	static inline const float kVelocityBias = 3.0f;

	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin = {-25.f, 25.f, -25.f, 25.f};

public:
	/// <summary>
	/// 追従対象をセット
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Player* target) { target_ = target; }
	/// <summary>
	/// 瞬間合わせ
	/// </summary>
	void Reset();
	/// <summary>
	/// 移動範囲を指定するセッター
	/// </summary>
	/// <param name="area"></param>
	void SetMovableArea(Rect area) { movableArea_ = area; }

};

