#pragma once

#include "Player.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "MyMath.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"

#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "vector"
#include "DebugCamera.h"

//スライド27～

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
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();

	/// <summary>
	/// ブロックの更新
	/// </summary>
	void UpdateBlocks();

	//すべての当たり判定を行う
	void CheckAllCollisions();

	/// <summary>
	/// フェーズの切り替え
	/// </summary>
	void ChangePhase();

	/// <summary>
	/// 終了フラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return finished_; }

private: 
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

		
	//自キャラ
	Player* player_ = nullptr;
	//プレイヤー3Dモデル
	Model* playerModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//3Dモデルデータ
	Model* dataModel_ = nullptr;

	// ブロック用のワールドトランスフォーム
	// 複数並べるように配列にする
	//std::vectorを重ねることで二次元配列となる
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//デバッグカメラの有効
	bool isDebugCameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	Skydome* skydome_ = nullptr;

	//3Dモデル描画
	Model* modelSkydome_ = nullptr;

	MapChipField* mapChipField_ = nullptr;

	//カメラコントローラ
	CameraController* cameraController_ = nullptr;

	//敵
	std::list<Enemy*> enemies_;

	//デスパーティクル
	Model* deathParticleModel_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;


	// ゲームのフェーズ(型）
	enum class Phase {
		//フェードイン
		kFadeIn,
		// ゲームプレイ
		kPlay,
		// デス演出
		kDeath,
		//フェードアウト
		kFadeOut
	};

	//現在のフェーズ
	Phase phase_;

	bool finished_ = false;
};
