#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "ImGuiManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	//--------------------------------
	// テクスチャ読み込み
	//--------------------------------
	
	//ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = TextureManager::Load("sample.png");

	//--------------------------------
	// 生成
	//--------------------------------
	//3Dモデルデータの生成
	// プレイヤー
	//model_.reset(Model::CreateFromOBJ("player",true));
	modelFighterBody_.reset(Model::CreateFromOBJ("Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("Head", true));
	modelFighterRightArm_.reset(Model::CreateFromOBJ("RightArm", true));
	modelFighterLightArm_.reset(Model::CreateFromOBJ("LeftArm", true));
	modelFighterWeapon_.reset(Model::CreateFromOBJ("Hammer", true));

	//敵
	modelEnemy_.reset(Model::CreateFromOBJ("Enemy", true));
	modelEnemyWeapon_.reset(Model::CreateFromOBJ("Weapon", true));

	//天球のモデルを生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));

	//地面のモデルデータを生成
	modelGround_.reset(Model::CreateFromOBJ("ground", true));

	//自キャラの生成
	player_ = std::make_unique<Player>();

	//敵の生成
	enemies_.emplace_back(std::make_unique<Enemy>());

	//天球を生成
	skydome_ = std::make_unique<Skydome>();

	//地面の生成
	ground_ = std::make_unique<Ground>();

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	//追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();

	//ロックオンの生成
	lockOn_ = std::make_unique<LockOn>();

	//--------------------------------
	// モデルデータをモデルデータ配列に格納
	//--------------------------------
	//自キャラモデル
	std::vector<Model*> playerModels = {modelFighterBody_.get(), modelFighterHead_.get(), modelFighterRightArm_.get(), modelFighterLightArm_.get(), modelFighterWeapon_.get()};

	//敵キャラのモデル
	std::vector<Model*> enemyMoldels = {modelEnemy_.get(),modelEnemyWeapon_.get()};

	//--------------------------------
	// 初期化
	//--------------------------------
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの初期化
	player_->Initialize(playerModels);

	//敵の初期化
	for (std::list<std::unique_ptr<Enemy>>::iterator enemy = enemies_.begin(); enemy != enemies_.end(); ++enemy) {
		(*enemy)->Initialize(enemyMoldels);
	}
	

	//天球を初期化
	skydome_->Initialize(modelSkydome_.get(),&viewProjection_);

	//地面の初期化
	ground_->Initialize(modelGround_.get(), &viewProjection_);

	//追従カメラの初期化
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//ロックオンの初期化
	lockOn_->Initalize();

	//自キャラに追従カメラのビュープロジェクションをアドレス渡しする
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// それぞれのロックオンを設定
	followCamera_->SetLockOn(lockOn_.get());
	player_->SetLockOn(lockOn_.get());
	//--------------------------------
	// 軸方向表示の使用
	//--------------------------------
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() {
	//--------------------------------
	// 更新処理関数を呼び出し
	//--------------------------------
    //自キャラの更新
	player_->Update();

	//敵キャラの更新
	for (std::list<std::unique_ptr<Enemy>>::iterator enemy = enemies_.begin(); enemy != enemies_.end(); ++enemy) {
		(*enemy)->Update();
	}

	//天球の更新
	skydome_->Update();

	//地面の更新
	ground_->Update();

	//ロックオンの更新
	lockOn_->Update(enemies_,viewProjection_);

	//追従カメラの更新
	//followCamera_->Update();

	//--------------------------------
	// デバッグカメラ
	//--------------------------------

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_C)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}

#endif // _DEBUG

	if (isDebugCameraActive_) {
		//追従カメラの更新
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;

		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;

		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
}
	

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);

	for (std::list<std::unique_ptr<Enemy>>::iterator enemy = enemies_.begin(); enemy != enemies_.end(); ++enemy) {
		(*enemy)->Draw(viewProjection_);
	}

	skydome_->Draw();

	ground_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//スプライトの描画
	//sprite_->Draw();

	//ロックオンの描画
	lockOn_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
