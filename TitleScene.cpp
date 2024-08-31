#include "TitleScene.h"
#include "Input.h"
#include <cassert>
#include <algorithm>

TitleScene::~TitleScene() { 
	delete modelTitle_;
	delete modelUI_;
	delete fade_;
}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	// 各種初期化
	modelTitle_ = Model::CreateFromOBJ("title", true);
	modelUI_ = Model::CreateFromOBJ("titleUI", true);

	assert(modelTitle_);
	assert(modelUI_);

	worldTransformTitle_.Initialize();
	worldTransformUI_.Initialize();

	worldTransformTitle_.translation_ = {0.f, 10.0f, 0.0f};
	worldTransformTitle_.rotation_ = {3.5f, 0.0f, 0.0f};
	worldTransformTitle_.scale_ = {2.0f, 2.0f, 2.0f};

	worldTransformUI_.translation_ = {0.0f, -9.0f, 0.0f};
	worldTransformUI_.rotation_ = {3.1f, 0.0f, 0.0f};

	viewProjection_.Initialize();

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 5.f);
}

void TitleScene::Update() { 

	switch (phase_) {
	case TitleScene::Phase::kFadeIn:
	case TitleScene::Phase::kFadeOut:

		fade_->Update();

		break;
	case TitleScene::Phase::kMain:

		break;

	default:
		break;
	}

	ChangePhase();

	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);
	//Aボタンでタイトルシーンを終了
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
		finished_ = true;
	}

	if (isIncreasing_) {
		counter_ += 1.0f / 60.0f; // 1秒で60回呼び出されると仮定
		if (counter_ >= kDuration) {
			counter_ = kDuration; // 上限に達したら固定
			isIncreasing_ = false; // 減少に転じる
		}
	} else {
		counter_ -= 1.0f / 60.0f; // 減少
		if (counter_ <= 0.0f) {
			counter_ = 0.0f;     // 下限に達したら固定
			isIncreasing_ = true; // 増加に転じる
		}
	}

	// ファードアウト
	color_.w = std::clamp((kDuration - counter_) / kDuration, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	// 色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();

	worldTransformTitle_.UpdateMatrix();
	worldTransformUI_.UpdateMatrix();
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
	modelTitle_->Draw(worldTransformTitle_, viewProjection_);
	modelUI_->Draw(worldTransformUI_, viewProjection_,&objectColor_);

	fade_->Draw(commandList);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion
}

void TitleScene::ChangePhase() {
	switch (phase_) {
	case TitleScene::Phase::kFadeIn:

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}

		break;
	case TitleScene::Phase::kMain:

		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			// フェードアウト開始
			float duration = 3.0f;
			fade_->Start(Fade::Status::FadeOut, duration);
			phase_ = Phase::kFadeOut;
		}

		break;
	case TitleScene::Phase::kFadeOut:

		// フェードアウトが終了したら、シーン終了
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		break;
	default:
		break;
	}
}