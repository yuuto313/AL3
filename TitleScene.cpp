#include "TitleScene.h"
#include "Input.h"

void TitleScene::Initialize() {
}

void TitleScene::Update() { 
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);
	//Aボタンでタイトルシーンを終了
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
		finished_ = true;
	}
}

void TitleScene::Draw() {}
