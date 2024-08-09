#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	//3Dモデルデータの解放
	delete model_;
	//自キャラ解放
	delete player_;
	//敵キャラk解放
	delete enemy_;
	//デバッグカメラ解放
	delete debugCamera_;
	//衝突判定マネージャの解放
	delete collisionManager_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	
	//-------------生成-------------//
	
	//３Dモデルデータの生成
	model_ = Model::Create();

	//自キャラの生成
	player_ = new Player();

	//敵キャラ生成
	enemy_ = new Enemy();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//衝突判定マネージャの生成
	collisionManager_ = new CollisionManager();

	//-------------初期化-------------//
	
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);

	//敵キャラ初期化
	enemy_->Initialize(model_,pos_,approachVelocity_,leaveVelocity_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	//自キャラの更新
	player_->Update();
	//敵キャラ更新
	enemy_->Update();
	//当たり判定
	collisionManager_->CheckAllCollisions(player_,enemy_);

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}

#endif // _DEBUG

	if (isDebugCameraActive_) {
	//デバッグカメラの更新
	debugCamera_->Update();
	viewProjection_.matView = debugCamera_->GetViewProjection().matView;

	viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();
	} else {
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

	//自キャラの描画
	player_->Draw(viewProjection_);
	//敵キャラ更新
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();



#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

