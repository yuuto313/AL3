#include "EnemyCanon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Enemy.h"

void EnemyCanon::Initialize(Model* model, Enemy* enemy, const Vector3& velocity) { 

	model_ = model;
	enemy_ = enemy;
	velocity_ = velocity;

	textureHandel_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	//　敵の中心座標を取得
	worldTransform_.translation_ = enemy_->GetWorldPosition();
}

void EnemyCanon::Update() {
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	float height = 8.49f;

	ImGui::Begin("Canon");
	ImGui::SliderFloat("Height", &height, 0.0f, 10.0f);
	ImGui::End();

	// 高さを調整
	worldTransform_.translation_.y = height;

	// 座標を移動させる
	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

}

void EnemyCanon::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandel_);

}