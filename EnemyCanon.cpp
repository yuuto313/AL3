#include "EnemyCanon.h"
#include "CollisionTypeIdDef.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Enemy.h"
#include "Player.h"

void EnemyCanon::Initialize(Model* model, Enemy* enemy, const Vector3& velocity) { 

	Collider::Initialize();

	model_ = model;
	enemy_ = enemy;
	velocity_ = velocity;

	worldTransform_.Initialize();
	//　敵の中心座標を取得
	worldTransform_.translation_ = enemy_->GetWorldPosition();

	// Y軸周りの回転角度(θy)を計算
	// atan2: 第一引数に高さ、第二引数に底辺を指定する
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	Vector3 newVelocity = velocity_;
	newVelocity.y = 0;
	float velocityXZ = Length(newVelocity);

	// X軸周りの回転角度(θx)を計算
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);

	// 種別IDの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemyCanon));

	Collider::SetRadius(3.0f);

}

void EnemyCanon::Update() {
	//--------------------------------
	// 敵弾のホーミング
	//--------------------------------
	
	// 敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();

	 // 正規化して方向ベクトルに変換
	toPlayer = Normalize(toPlayer);

	// ホーミング強度 (値を調整してホーミングの強さを変えられます)
	float homingStrength = 0.05f;
	// 敵弾の速さ
	float speed = 0.9f;

	// 現在の速度にプレイヤー方向を加味して補間
	velocity_ = Sleap(velocity_, toPlayer, homingStrength) * speed;

	//--------------------------------
	// 弾が進行方向に向くように回転角度を更新
	//--------------------------------

	// Y軸周りの回転角度 (θy) を計算
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	// X軸周りの回転角度 (θx) を計算
	Vector3 velocityXZ = velocity_;
	velocityXZ.y = 0;
	float lengthXZ = Length(velocityXZ);
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, lengthXZ);

	//--------------------------------
	// 時間経過でデス
	//--------------------------------

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

	model_->Draw(worldTransform_, viewProjection);

}

Vector3 EnemyCanon::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyCanon::OnCollision(Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手がプレイヤーなら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		Player* player = static_cast<Player*>(other);
		player->Reaction(damage_);
	}
}

Vector3 EnemyCanon::GetCenterPosition() const { 
	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}
