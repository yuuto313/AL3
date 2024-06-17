#pragma once
#include "ViewProjection.h"
/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection& viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	ViewProjection viewProjection_;
};
