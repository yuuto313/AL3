#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include "Enemy.h"

class LockOn;

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 設定したターゲットに追従する
	/// </summary>
	void FollowTarget();
	/// <summary>
	/// カメラの旋回
	/// </summary>
	void RotateCamera();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 追従対象からのオフセットを計算する
	/// </summary>
	/// <returns></returns>
	Vector3 CalculateOffset() const;

	/// <summary>
	/// 自キャラのWorldTransformを取得する
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ロックオンセッター
	/// </summary>
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	/// <summary>
	/// 追従対象
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);

private:
	ViewProjection viewProjection_;

	//追従対象
	const WorldTransform* target_ = nullptr;

	//ロックオン
	const LockOn* lockOn_ = nullptr;

	// 目標角度
	float destinationAngleY_ = 0.0f;

	//追従対象の残像座標
	Vector3 interTarget_ = {};
};
