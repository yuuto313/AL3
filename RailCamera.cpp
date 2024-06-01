#include "RailCamera.h"
#include "imgui.h"
void RailCamera::Initialize(Vector3 position,Vector3 rotate) {
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//ワールドトランスフォームの座標、角度の数値を加算
	worldTransform_.translation_ += kSpeed;
	worldTransform_.rotation_ += kRotate;

	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateMatrix();

	//カメラの座標を画面表示
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, 0.0f,1.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -1.0f,1.0f);
	ImGui::End();
}



