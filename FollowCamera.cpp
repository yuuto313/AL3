#include "FollowCamera.h"
#include "ImGuiManager.h"
#include "LockOn.h"

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

}

void FollowCamera::FollowTarget() {

	if (target_) {

		// 追従対象からカメラまでのオフセット(0度の時の値)
		Vector3 offset = {0.0f, 5.0f, -20.0f};

		// カメラの回転角度を取得
		Vector3 rotationAngle = {viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z};

		////カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotationAngle.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotationAngle.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotationAngle.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		// オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, rotateXYZMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}


	// ロックオン中
	if (lockOn_) {
		// ロックオン座標
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		// 対風対象からロックオン対象へのベクトル
		Vector3 sub = lockOnPosition - target_->translation_;
		// Y軸周り角度
		viewProjection_.rotation_.y = std::atan2(sub.x, sub.z);
	} else {
		//--------------------------------
		// カメラ旋回処理
		//--------------------------------

		RotateCamera();
	}

}

void FollowCamera::RotateCamera() {
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float rotate = 0.03f;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotate;
	}
}
