#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <sstream>
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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 敵の生成
	/// </summary>
	/// <param name="position"></param>
	void Spawn(Vector3 position);

	private:
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	//3Dモデルデータ
	Model* model_ = nullptr;
	Model* reticleModel_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ビュープロジェクション
	ViewProjection viewProjection_;
	
	//デバッグカメラ有効
	bool isDebugCameraActive_ = true;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	std::list<Enemy*> enemies_;
	
	//天球
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//カメラオブジェクト
	RailCamera* railCamera_ = nullptr;

	// 敵弾リスト
	std::list<EnemyBullet*> enemyBullets_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	Vector3 enemyPosition_;

	//待機中フラグ
	static inline bool isWait = false;
	
	//待機タイマー
	static inline int32_t waitTimer = 0;
};
