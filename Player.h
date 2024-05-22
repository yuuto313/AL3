#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#pragma once
/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);
	/// <summary>
	/// 旋回（回転）
	/// </summary>
	void Rotate();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	//ImGuiで値を入力する変数
	float inputFloat3[3] = {worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z};

	//弾
	std::list<PlayerBullet*> bullets_;
};
