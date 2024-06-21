#include "Player.h"
#include "iostream"

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

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}

void Player::Update() { 
	//--------------------------------
	//移動
	//--------------------------------

	Move();

	//--------------------------------
	//ワールド行列の転送
	//--------------------------------
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); 
}

void Player::Move() {
	//--------------------------------
	//コントローラーで移動処理
	//--------------------------------
	//速さ 
	const float speed = 0.3f;
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		
		//移動量
		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		//移動量に速さを反映
		move = move * speed;

		// カメラの回転角度を取得

		Vector3 rotationAngle = {GetViewProjection()->rotation_.x, GetViewProjection()->rotation_.y, GetViewProjection()->rotation_.z};

		////カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotationAngle.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotationAngle.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotationAngle.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		//移動ベクトルをカメラの座標だけ回転する
		move = TransformNormal(move, rotateXYZMatrix);
		
		//移動
		worldTransform_.translation_ += move;

		//--------------------------------
		// 移動方向に見た目を合わせる
		//--------------------------------

		//Y軸周りの角度
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}
}



