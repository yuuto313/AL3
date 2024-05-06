#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	//解放

	//02_01

	delete playerModel_;
	delete player_;

	//02_02

	delete dataModel_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;

	//02_03
	delete skydome_;
	delete modelSkydome_;

	delete mapChipField_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	//02_01

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");

	//プレイヤー3Dモデルデータの生成
	playerModel_ = Model::Create();


	//ビュープロジェクションの初期化

	//farZを適度に大きい値に変更する
	//viewProjection_.farZ = 400.f;
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Init(playerModel_,textureHandle_,&viewProjection_);

	//02_02

	dataModel_ = Model::Create();

	//// 要素数
	//const uint32_t kNumBlockVirtical = 10;
	//const uint32_t kNumBlockHorizontal = 20;
	//// ブロック一個分の横幅
	//const float kBlockWidth = 2.0f;
	//const float kBlockHeight = 2.0f;

	//const float kBlockWidthSpace = 2.0f;
	//const float kBlockHeightSpace = 2.0f;

	//// 要素数を変更する
	////列数を設定（縦方向のブロック数）
	//worldTransformBlocks_.resize(kNumBlockVirtical);
	//for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
	//	// 1列の要素数を設定（横方向のブロック数）
	//	worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//}

	//// キューブの生成
	//for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
	//	for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
	//		worldTransformBlocks_[i][j] = new WorldTransform();
	//		worldTransformBlocks_[i][j] ->Initialize();
	//		worldTransformBlocks_[i][j] ->translation_.x = kBlockWidth * j * kBlockWidthSpace;
	//		worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i * kBlockHeightSpace;
	//	}
	//}

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	
	//02_03
	

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

    skydome_ = new Skydome();
	skydome_->Init(modelSkydome_,&viewProjection_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("./Resources/AL3.csv");

	GenerateBlocks();

}

void GameScene::Update() {

	// 02_01

	// 自キャラの更新
	player_->Update();

	// 02_02

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}

#endif // _DEBUG

	//デバッグカメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_ -> GetViewProjection().matView;

		viewProjection_.matProjection = debugCamera_ -> GetViewProjection().matProjection;
		
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} 
	else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	//02_03
	skydome_->Update();

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
	
	

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) 
				continue;
			    dataModel_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

    //3Dモデル描画
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

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
