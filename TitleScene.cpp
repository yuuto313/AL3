#include "TitleScene.h"
#include "Input.h"
#include <cassert>
TitleScene::~TitleScene() {
	delete titleModel_;
	delete UIModel_;
	delete fade_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	// 各種初期化
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

	float duration = 3.0f;

	fade_ = new Fade();
	fade_->Initialize();
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

	// モデル描画

	titleModel_->Draw(titleWorldTransform_, viewProjection_);
	UIModel_->Draw(UIWorldTransform_, viewProjection_);

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
}

void TitleScene::ChangePhase() {
	
}