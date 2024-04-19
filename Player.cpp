#include "Player.h"



Player::Player() {}

Player::~Player() {delete debugCamera_;}

void Player::Init(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) { 
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	//ワールド変換初期化
	worldTransform_.Initialize();

	// 一応追加
	debugCamera_ = new DebugCamera(1280, 720);

}

void Player::Update() { 
	//ワールド行列の転送
	worldTransform_.TransferMatrix(); 

	debugCamera_->Update();

}

void Player::Draw() { 
	//model_->Draw(worldTransform_, *viewProjection_, textureHandle_); 
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
}



