#include "FollowCamera.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	//--------------------------------
	//追従処理
	//--------------------------------

	FollowTarget();

	//--------------------------------
	//ビュー行列の更新と転送
	//--------------------------------
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

void FollowCamera::FollowTarget() {
	//追従対象がいれば
	if (target_) {
	//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}
}
