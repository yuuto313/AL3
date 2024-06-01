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

	worldTransform_.translation_ += {0.0f, 0.0f, -1.0f};
	worldTransform_.rotation_ += {1.0f, 1.0f, 1.0f};

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, 0.0f,1.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, 0.0f,1.0f);
	ImGui::End();
}



