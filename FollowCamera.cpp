#include "FollowCamera.h"
#include "ImGuiManager.h"
void FollowCamera::Initialize() { 
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	//--------------------------------
	//追従処理
	//--------------------------------

	FollowTarget();

	//--------------------------------
	// カメラ旋回処理
	//--------------------------------

	RotateCamera();

	//--------------------------------
	//ビュー行列の更新と転送
	//--------------------------------
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("debugCamera.translation", &viewProjection_.translation_.x, 0.03f);
	ImGui::DragFloat3("debugCamera.rotate", &viewProjection_.rotation_.x, 0.03f);
	ImGui::End();

}

void FollowCamera::FollowTarget() {
	//追従対象がいれば
	if (target_) {
	//追従対象からカメラまでのオフセット(0度の時の値)
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

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}
}

void FollowCamera::RotateCamera() {
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float rotate = 0.03f;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotate;
	}
}
