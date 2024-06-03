#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	//--------------------------------
	//解放処理
	//--------------------------------
	delete model_;
	delete player_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	
	//--------------------------------
	//newと生成の処理
	//--------------------------------
	// 天球を生成
	skydome_ = new Skydome();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// カメラオブジェクト
	railCamera_ = new RailCamera();

	// 自キャラの生成
	player_ = new Player();

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ３Dモデルデータの生成
	model_ = Model::Create();

	//--------------------------------
	//Initializeの処理
	//--------------------------------
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	railCamera_->Initialize(viewProjection_.translation_, viewProjection_.rotation_);

	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// 自キャラの初期化
	// z = カメラから前にずらす量
	Vector3 playerPosition(0.0f, 0.0f, 20.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	LoadEnemyPopData();
}

void GameScene::Update() {

	//--------------------------------
	//それぞれの更新
	//--------------------------------
	UpdateEnemyPopCommands();
	//天球の更新
	skydome_->Update();
	//カメラオブジェクトの更新
	railCamera_->Update();
	//自キャラの更新
	player_->Update();
	//敵キャラ更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	//当たり判定
	CheckAllCollisions();
	//--------------------------------
	// デスフラグの立った弾を削除
	//--------------------------------

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//--------------------------------
	// デスフラグの立った敵を削除
	//--------------------------------

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	//--------------------------------
	// 攻撃処理
	//--------------------------------

	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	//--------------------------------
	//デバッグカメラの切り替え
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

	//--------------------------------
	//デバッグカメラの更新
	//--------------------------------

	if (isDebugCameraActive_) {
	debugCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;

	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

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

	for (EnemyBullet* bullet :enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	//自キャラの描画
	player_->Draw(viewProjection_);
	//敵キャラの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	//天球の描画
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


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	if (player_->GetBullets().empty() || enemies_.empty() || enemyBullets_.empty())
		return;

	// 判定対象AとBの座標
	Vector3 posA;
	Vector3 posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerbullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*> enemybullets = enemies_.front()->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラの敵弾すべての当たり判定

	for (EnemyBullet* bullet : enemybullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float distance = Length(posB - posA);
		float playerRadius = player_->GetRadius();
		float enemyBulletRadius = bullet->GetRadius();
		// 弾と弾の交差判定
		if (distance <= playerRadius + enemyBulletRadius) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}


#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// すべての敵キャラとすべての自弾の当たり判定
	for (Enemy* enemy : enemies_) {
		for (PlayerBullet* bullet : playerbullets) {
			// 敵キャラの座標
			posA = enemy->GetWorldPosition();
			// 自弾の座標
			posB = bullet->GetWorldPosition();
			// 座標AとBの距離を求める
			float distance = Length(posB - posA);
			float enemyRadius = enemy->GetRadius();
			float playerBulletRadius = bullet->GetRadius();
			// 弾と弾の交差判定
			if (distance <= enemyRadius + playerBulletRadius) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 敵キャラの自弾すべての当たり判定
	for (PlayerBullet* playerBullet : playerbullets) {
		for (EnemyBullet* enemyBullet : enemybullets) {
			// 自弾の座標
			posA = playerBullet->GetWorldPosition();
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			float distance = Length(posB - posA);
			float playerBulletRadius = playerBullet->GetRadius();
			float enemyBulletRadius = enemyBullet->GetRadius();

			// 弾と弾の交差判定
			if (distance <= playerBulletRadius + enemyBulletRadius) {
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}


void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources./enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
		//待機完了
			isWait = false;
		}
		return;
	}

	// １行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// １行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得する
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			Spawn({x, y, z});
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;

		}
	}
}

void GameScene::Spawn(Vector3 position) { 
	Enemy* enemy = new Enemy();
	enemy->Initialize(model_, position);
	enemy->SetGameScene(this);
	enemy->SetPlayer(player_);
	enemies_.push_back(enemy);
}
