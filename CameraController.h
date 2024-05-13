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
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

private:
	ViewProjection viewProjection_;

	//追従対象となるプレイヤーから継続的に座標を取得するためにプレイヤーのポインタを持たせる
	Player* target_ = nullptr;

	//追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0.f, 0.f, -15.0f};


};

