#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"
Player::Player() {}

Player::~Player() {

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();

}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変数の初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 要素数
	const uint32_t kNumBlockHorizontal = 1;
	// ブロック一個分の横幅
	const float kBlockWidth = 2.0f;

	// 要素数を変更する
	// 列数を設定（縦方向をのブロック数）
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	for (uint32_t i = 0;i < kNumBlockHorizontal; ++i) {
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
}


void Player::Update() {

	//--------------------------------
	// キーボード入力によって移動ベクトルを変更する処理
	//--------------------------------
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	const float kcharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更する（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kcharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kcharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更する（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kcharacterSpeed;
	} else if (input_->PushKey(DIK_LEFT)) {
		move.y -= kcharacterSpeed;
	}
	// 座標移動(ベクトルの加算）
	worldTransform_.translation_ += move;

	//--------------------------------
	//移動制限
	//--------------------------------
	//移動限界座標
	const float kMoveLimitX = 600;
	const float kMoveLimitY = 300;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	//--------------------------------
	//行列の更新
	//--------------------------------
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		worldTransformBlock->UpdateMatrix();

		// 定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}

	// 行列を定数バッファに転送する
	worldTransform_.TransferMatrix();

	//--------------------------------
	//ImGuiで座標表示
	//--------------------------------
	//#ifdef _DEBUG
//	ImGui::Begin("");
//	ImGui::End();
//#endif // _DEBUG

}

void Player::Draw(ViewProjection& viewProjection) {

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		model_->Draw(*worldTransformBlock, viewProjection, textureHandle_);
	}

}
