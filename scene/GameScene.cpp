#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	//解放

	delete playerModel_;
	delete player_;

	delete dataModel_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;

	delete skydome_;
	delete modelSkydome_;

	delete mapChipField_;

	delete cameraController_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	delete deathParticleModel_;
	delete deathParticles_;
	delete fade_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	//プレイヤー3Dモデルデータの生成
	playerModel_ = Model::CreateFromOBJ("player", true);


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("./Resources/AL3.csv");
	GenerateBlocks();


    //自キャラの生成
	player_ = new Player();
	//座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	//自キャラの初期化
	player_->Initialize(playerModel_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	dataModel_ = Model::Create();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

    skydome_ = new Skydome();
	skydome_->Init(modelSkydome_,&viewProjection_);

	//生成
	cameraController_ = new CameraController;
	//初期化
	cameraController_->Initialize(&viewProjection_);
	//追従対象をセット
	cameraController_->SetTarget(player_);
	//リセット（瞬間合わせ）
	cameraController_->Reset();
	//移動範囲の指定
	cameraController_->SetMovableArea({20.f, 50.f, 0.f, 100.f});

	Vector3 positions[] = {mapChipField_->GetMapChipPositionByIndex(20, 18), mapChipField_->GetMapChipPositionByIndex(30, 18), mapChipField_->GetMapChipPositionByIndex(35, 18)};

	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = positions[i];
		newEnemy->Initialize(playerModel_,&viewProjection_,enemyPosition);
		enemies_.push_back(newEnemy);
	}

	//デスパーティクルのモデル
	deathParticleModel_ = Model::CreateFromOBJ("deathParticle", true);

	//フェードの生成、初期化
	fade_ = new Fade();
	fade_->Initialize();
	float duration = 1.5f;
	fade_->Start(Fade::Status::FadeIn,duration);

}

void GameScene::Update() {
	//	フェーズの切り替え
	ChangePhase();
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


    //3Dモデル描画
	skydome_->Draw();

	//プレイヤー描画
	player_->Draw();

	//敵描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	//プレイヤーのデスパーティクル描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	//ブロック描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			dataModel_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// フェードの描画
	switch (phase_) {
	case GameScene::Phase::kFadeIn:
	case GameScene::Phase::kFadeOut:

		fade_->Draw(commandList);

		break;
	case GameScene::Phase::kPlay:

		break;
	case GameScene::Phase::kDeath:

		break;
	default:
		break;
	}


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

void GameScene::UpdateCamera() {
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}

#endif // _DEBUG

	// デバッグカメラの処理
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

void GameScene::UpdateBlocks() {
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
}

void GameScene::CheckAllCollisions() {


	//判定対象1と2の座標
	AABB aabb1, aabb2;

	//自キャラの座標
	aabb1 = player_->GetAABB();

	//敵キャラすべての当たり判定
	for (Enemy* enemy : enemies_) {
		//敵キャラの当たり判定
		aabb2 = enemy->GetAABB();

		//AABB同士の交差判定
		if (IsCollisionAABB(aabb1, aabb2)) {
			//自キャラの衝突判定コールバックを呼び出す
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	}



}

void GameScene::ChangePhase() {

	switch (phase_) {
	case GameScene::Phase::kFadeIn:

		fade_->Update();

		// 自キャラの更新
		player_->Update();

		// 天球
		skydome_->Update();

		// カメラコントローラの更新
		cameraController_->Update();

		// カメラの更新
		UpdateCamera();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// ブロックの更新
		UpdateBlocks();


		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}

		break;

	case GameScene::Phase::kPlay:

		// 自キャラの更新
		player_->Update();

		// 天球
		skydome_->Update();

		// カメラコントローラの更新
		cameraController_->Update();

		//カメラの更新
		UpdateCamera();

		// 当たり判定を行う
		CheckAllCollisions();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// ブロックの更新
		UpdateBlocks();

		//フェーズの切り替え
		if (player_->IsDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			// 自キャラの座標に	デスパーティクルを発生、初期化
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticleModel_, &viewProjection_, deathParticlesPosition);
		}

		break;
	case GameScene::Phase::kDeath:

		// デスパーティクルの更新
		if (deathParticles_) {
			deathParticles_->Update();
		}

		// 天球
		skydome_->Update();

		// カメラコントローラの更新
		cameraController_->Update();

		// カメラの更新
		UpdateCamera();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// ブロックの更新
		UpdateBlocks();

		//フェードアウト開始
		if (deathParticles_ && deathParticles_->IsFinished()) {
			float duration = 3.0f;
			fade_->Start(Fade::Status::FadeOut, duration);
			phase_ = Phase::kFadeOut;
		}

		break;

	case GameScene::Phase::kFadeOut:

		fade_->Update();

		if (fade_->IsFinished()) {
			finished_ = true;
		}

		break;
	default:
		break;
	}
}
