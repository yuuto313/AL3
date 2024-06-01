#include "PlayerBullet.h"
#include <cassert>
#include "TextureManager.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) { 
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャを読み込む
	textureHandle_ = TextureManager::Load("white1x1.png");
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update(){
	//--------------------------------
	// 時間経過でデス
	//--------------------------------
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	//--------------------------------
	// 行列の更新
	//--------------------------------
	worldTransform_.UpdateMatrix();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void PlayerBullet::OnCollision() { 
	//デスフラグを立てる
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}
