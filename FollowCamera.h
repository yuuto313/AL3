#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"


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
	/// 追従対象
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) { target_ = target; }
	/// <summary>
	/// 設定したターゲットに追従する
	/// </summary>
	void FollowTarget();
	/// <summary>
	/// カメラの旋回
	/// </summary>
	void RotateCamera();

	/// <summary>
	/// 自キャラのWorldTransformを取得する
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	private:
	ViewProjection viewProjection_;

	
	//追従対象
	const WorldTransform* target_ = nullptr;
};
