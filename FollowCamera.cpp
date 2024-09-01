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

	// 追従対象がいれば
	if (target_) {
		// 追従対象の補間
		interTarget_ = Lerp(interTarget_, target_->translation_, 0.8f);

		// 追従対象からカメラまでのオフセット(0度の時の値)
		Vector3 offset = CalculateOffset();

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = interTarget_ + offset;
	}

	// ロックオン中
	if (lockOn_ && lockOn_->ExistTarget()) {
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
	//--------------------------------
	//  旋回処理
	//--------------------------------
	XINPUT_STATE joyState;
	// 回転速度
	const float rotate = 0.03f;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotate;

		// 右スティック押し込みでリセットする
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			Reset();
		}
	}

	if (Input::GetInstance()->PushKey(DIK_LEFTARROW)) {
		viewProjection_.rotation_.y -= rotate;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHTARROW)) {
		viewProjection_.rotation_.y += rotate;
	}

	// 最短角度補間
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.2f);
}

void FollowCamera::Reset() {
	//追従対象がいれば
	if (target_) {
		//追従対象・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;	
	}
	destinationAngleY_ = viewProjection_.rotation_.y;

	//追従対象からのオフセット
	Vector3 offset = CalculateOffset();
	viewProjection_.translation_ = interTarget_ + offset;
}

Vector3 FollowCamera::CalculateOffset() const {
	// 追従対象からのオフセット
	Vector3 offset = {0.0f, 5.0f, -20.0f};

	// カメラの回転角度を取得
	Vector3 rotationAngle = {viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z};

	// 回転行列の合成
	//  カメラの角度から回転行列を計算する
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotationAngle.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotationAngle.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotationAngle.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	// オフセットをカメラの回転に合わせて回転させる
	offset = TransformNormal(offset, rotateXYZMatrix);

	return offset;
}

void FollowCamera::SetTarget(const WorldTransform* target) {
	target_ = target;
	Reset();
}
