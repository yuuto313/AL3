#include "Fade.h"
#include "WinApp.h"
#include <algorithm>
void Fade::Initialize() {
	sprite_ = new Sprite();
	sprite_->Initialize();
	sprite_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Update() {
	//フェード状態による分岐
	switch (status_) {
	case Status::None:
		//何もしない
		break;
	case Status::FadeIn:

		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.f;
		// フェード経過時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード経過時間に近づくほどα値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1.0f-(counter_ / duration_), 0.0f, 1.0f)));


		break;
	case Status::FadeOut:

		//1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.f;
		//フェード経過時間に達したら打ち止め
		if (counter_>=duration_) {
			counter_ = duration_;
		}
		//0.0fから1.0fの間で、経過時間がフェード経過時間に近づくほどα値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0,std ::clamp(counter_ / duration_, 0.0f, 1.0f)));

		break;
	default:
		break;
	}
}

void Fade::Draw(ID3D12GraphicsCommandList* commandList) { 
	//スプライトの描画
	Sprite::PreDraw(commandList); 
	sprite_->Draw();
	Sprite::PostDraw();
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() {
	status_ = Status::None;
	if (status_ == Status::None) {
		return;
	}
}

bool Fade::IsFinished() const {

	//フェード状態による分岐
	switch (status_) {
	case Fade::Status::None:
		break;
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:

		if (counter_ >= duration_) {
			return true;			
		} else {
			return false;
		}

		break;
	default:
		break;
	}

	return true;
}
