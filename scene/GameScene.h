#include "Player.h"
#include "Enemy.h"

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Collider.h"
#include "CollisionManager.h"
#pragma once



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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//-------------ポインタ類-------------//

	//3Dモデルデータ
	Model* model_ = nullptr;
	
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	//衝突判定マネージャ
	CollisionManager* collisionManager_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 敵キャラの座標
	Vector3 pos_ = {2.0f, 2.0f, 50.0};
	// 敵キャラの速度
	Vector3 approachVelocity_ = {0.0f, 0.0f, -0.05f};
	Vector3 leaveVelocity_ = {0.1f, 0.1f, -0.05f};
	
	

};
