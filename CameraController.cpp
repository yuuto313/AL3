#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include "vector"
#include <algorithm>
#include "MyMath.h"

CameraController::CameraController() {}

CameraController::~CameraController() {}

void CameraController::Initialize(ViewProjection* viewProjection) { 
	viewProjection_ = viewProjection;
}

void CameraController::Update() {
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform(); 
	//追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + target_->GetVelocity() * kVelocityBias;

	//座標変換によりゆったり追従
	viewProjection_->translation_ = Lerp(viewProjection_->translation_, targetPosition_, kInterpolationRate);

	//追従対象が画面外に出ないように補正
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, targetWorldTransform.translation_.x + margin.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, targetWorldTransform.translation_.x + margin.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, targetWorldTransform.translation_.y + margin.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, targetWorldTransform.translation_.y + margin.top);

	//移動範囲制限
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, movableArea_.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, movableArea_.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, movableArea_.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, movableArea_.top);

	//行列を更新
	viewProjection_->UpdateMatrix();

}

void CameraController::Reset() {
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	//追従対象とオフセットからカメラ座標を計算
	viewProjection_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}
