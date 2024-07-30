#include "TitleScene.h"
#include <cassert>
#include "Input.h"
TitleScene::~TitleScene() { 
	delete model_; 
	delete fade_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	//各種初期化
	model_ = Model::CreateFromOBJ("title", true);
	assert(model_);
	worldTransform_.Initialize();
	worldTransform_.rotation_ = {90.f, 0.0f, 0.0f};
	viewProjection_.Initialize();
	fade_ = new Fade();
	fade_->Initialize();
	float duration = 2.0f;
	fade_->Start(Fade::Status::FadeIn, duration);
}

void TitleScene::Update() { 

	switch (phase_) {
	case TitleScene::Phase::kFadeIn:

		fade_->Update();

		// フェードイン中にフェードが終わったらメインフェーズに切り替える
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}

		break;
	case TitleScene::Phase::kMain:

		// スペースでフェードアウトを開始する
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			phase_ = Phase::kFadeOut;
		}

		break;
	case TitleScene::Phase::kFadeOut:

		fade_->Update();

		// フェードアウト中にフェードが終了したらタイトルシーンを終了する
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		break;
	default:
		break;
	}

	fade_->Update();

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

	switch (phase_) {
	case TitleScene::Phase::kFadeIn:

		fade_->Draw(commandList);

		break;
	case TitleScene::Phase::kMain:
		break;
	case TitleScene::Phase::kFadeOut:

		fade_->Draw(commandList);

		break;
	default:
		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion
	
}

void TitleScene::ChangePhase() {

}
