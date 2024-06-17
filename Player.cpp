#include "Player.h"



Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model,ViewProjection* viewProjection) { 
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	//ワールド変換初期化
	worldTransform_.Initialize();

}

void Player::Update() { 
	//--------------------------------
	//移動
	//--------------------------------

	Move();

	//--------------------------------
	//ワールド行列の転送
	//--------------------------------
	worldTransform_.TransferMatrix(); 
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); 
}

void Player::Move() {
	//--------------------------------
	//コントローラーで移動処理
	//--------------------------------

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//速さ
		const float speed = 0.3f;

		//移動量
		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		//移動量に速さを反映
		move = Normalize(move) * speed;

		//移動
		worldTransform_.translation_ += move;
	}


	//--------------------------------
	// キーボードで移動処理
	//--------------------------------

}



