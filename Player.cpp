#include "Player.h"
#include <cassert>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	
	//ワールド変数の初期化
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {

	
	//行列を定数バッファに転送する
	worldTransform_.TransferMatrix();

	//--------------------------------
	//キーボード入力によって移動ベクトルを変更する処理
	//--------------------------------
	//キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	//キャラクターの移動速さ
	const float kcharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更する（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kcharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kcharacterSpeed;
	}

	//押した方向で移動ベクトルを変更する（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kcharacterSpeed;
	} else if (input_->PushKey(DIK_LEFT)) {
		move.y -= kcharacterSpeed;
	}
	//座標移動(ベクトルの加算）
	worldTransform_.translation_ += move;
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
