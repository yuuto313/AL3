#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "DebugCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "LockOn.h"
#include "CollisionManager.h"

#include <memory>

//カメラの挙動修正
//コントローラーとキーボードの操作

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollsions();

private:
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;
	
	// 敵
	std::list<std::unique_ptr<Enemy>> enemies_;

	//スカイドーム（天球）
	std::unique_ptr<Skydome> skydome_ = nullptr;

	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	// 3Dモデルデータ

	// 3Dモデル
	// プレイヤー
	//std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Model> modelFighterBody_ = nullptr;
	std::unique_ptr<Model> modelFighterHead_ = nullptr;
	std::unique_ptr<Model> modelFighterLightArm_ = nullptr;
	std::unique_ptr<Model> modelFighterRightArm_ = nullptr;
	std::unique_ptr<Model> modelFighterWeapon_ = nullptr;

	//敵
	std::unique_ptr<Model> modelEnemy_ = nullptr;
	std::unique_ptr<Model> modelEnemyWeapon_ = nullptr;

	//天球のモデル
	std::unique_ptr<Model> modelSkydome_ = nullptr;

	//地面のモデル
	std::unique_ptr<Model> modelGround_ = nullptr;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	//ロックオン
	std::unique_ptr<LockOn> lockOn_;

	//衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	//テクスチャハンドル
	//uint32_t textureHandle_ = 0;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
