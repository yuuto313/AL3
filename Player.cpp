#include "Player.h"

Player::Player() {}

Player::~Player() {}

void Player::Init(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) { 
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	//ワールド変換初期化
	worldTransform_.Initialize();

}

void Player::Update() { 
	//ワールド行列の転送
	worldTransform_.TransferMatrix(); 
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_); 
}



