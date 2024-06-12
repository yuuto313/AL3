#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	//--------------------------------
	// テクスチャ読み込み
	//--------------------------------
	
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");

	//--------------------------------
	// 生成
	//--------------------------------
	//3Dモデルデータの生成
	model_.reset(Model::Create());

	//天球のモデルを生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));

	//自キャラの生成
	player_ = std::make_unique<Player>();

	//天球を生成
	skydome_ = std::make_unique<Skydome>();

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	//--------------------------------
	// 初期化
	//--------------------------------
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの初期化
	player_->Initialize(model_.get(), textureHandle_, &viewProjection_);

	//天球を初期化
	skydome_->Initialize(modelSkydome_.get(),&viewProjection_);


}

void GameScene::Update() {
	//--------------------------------
	// 更新処理関数を呼び出し
	//--------------------------------
    //自キャラの更新
	player_->Update();

	//天球の更新
	skydome_->Update();

	//--------------------------------
	// デバッグカメラ
	//--------------------------------

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
		// デバッグカメラの更新
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

	player_->Draw();

	skydome_->Draw();

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

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
