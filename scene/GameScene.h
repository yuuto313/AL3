#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"

#include "Player.h"
#include "Skydome.h"

#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;
	
	//スカイドーム（天球）
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 3Dモデル
	std::unique_ptr<Model> model_ = nullptr;

	//天球のモデル
	std::unique_ptr<Model> modelSkydome_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	

	//ビュープロジェクション
	ViewProjection viewProjection_;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
