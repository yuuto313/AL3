#include "TitleScene.h"
#include <cassert>
#include "Input.h"
TitleScene::~TitleScene() { 
	delete titleModel_; 
	delete UIModel_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	//各種初期化
	titleModel_ = Model::CreateFromOBJ("title", true);
	UIModel_ = Model::CreateFromOBJ("titleUI", true);

	assert(titleModel_);
	assert(UIModel_);

	titleWorldTransform_.Initialize();
	UIWorldTransform_.Initialize();

	titleWorldTransform_.translation_ = {0.f, 10.0f, 0.0f};
	titleWorldTransform_.rotation_ = {0.3f, 0.0f, 0.0f};

	UIWorldTransform_.translation_ = {0.0f, -9.0f, 0.0f};

	viewProjection_.Initialize();

}

void TitleScene::Update() { 
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	titleWorldTransform_.UpdateMatrix();
	UIWorldTransform_.UpdateMatrix();
}

void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//モデル描画
	titleModel_->Draw(titleWorldTransform_, viewProjection_);
	UIModel_->Draw(UIWorldTransform_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion
	
}
