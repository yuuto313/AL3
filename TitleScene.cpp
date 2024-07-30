#include "TitleScene.h"
#include <cassert>
#include "Input.h"
TitleScene::~TitleScene() { 
	delete model_; 
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	//各種初期化
	model_ = Model::CreateFromOBJ("title", true);
	assert(model_);
	worldTransform_.Initialize();
	worldTransform_.rotation_ = {90.f, 0.0f, 0.0f};
	viewProjection_.Initialize();

}

void TitleScene::Update() { 
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	worldTransform_.UpdateMatrix();
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
	model_->Draw(worldTransform_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion
	
}
