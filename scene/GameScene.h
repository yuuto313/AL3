#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "Player.h"
#include "MyMath.h"
#include "vector"

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

	//02_01
	
	//自キャラ
	Player* player_ = nullptr;
	//プレイヤー3Dモデル
	Model* playerModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//02_02

	//3Dモデルデータ
	Model* dataModel_ = nullptr;

	// ブロック用のワールドトランスフォーム
	// 複数並べるように配列にする
	std::vector<WorldTransform*> worldTransformBlocks_;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
