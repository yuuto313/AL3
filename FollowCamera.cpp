#include "FollowCamera.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	//--------------------------------
	//追従処理
	//--------------------------------

	//FollowTarget();

	//--------------------------------
	// カメラ旋回処理
	//--------------------------------

	RotateCamera();

	//--------------------------------
	//ビュー行列の更新と転送
	//--------------------------------
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

void FollowCamera::FollowTarget() {
	//追従対象がいれば
	//if (target_) {
	////追従対象からカメラまでのオフセット(0度の時の値)
	//	Vector3 offset = {0.0f, 2.0f, -10.0f};

	//	//カメラの角度から回転行列を計算する
	////https: // chatgpt.com/c/29947849-3dd7-4f56-93c8-47e009964be4

	//	// オフセットをカメラの回転に合わせて回転させる
	//	offset = TransformNormal(offset, 回転行列);

	//	//座標をコピーしてオフセット分ずらす
	//	viewProjection_.translation_ = target_->translation_ + offset;
	//}
}

void FollowCamera::RotateCamera() {
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float rotate = 0.3f;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX * rotate;
	}
}
