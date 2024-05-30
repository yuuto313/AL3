#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	//3Dモデルデータの開放
	delete model_;
	//自キャラ開放
	delete player_;
	//敵キャラ開放
	delete enemy_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ワールドトランスフォームの初期化
	//worldTransform_.Initialize();
	
	//３Dモデルデータの生成
	model_ = Model::Create();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);

	//敵キャラ生成
	enemy_ = new Enemy();
	//敵キャラ初期化
	enemy_->Initialize(model_,pos_,approachVelocity_,leaveVelocity_);
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

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

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA;
	Vector3 posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerbullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemybullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラの敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemybullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		//座標AとBの距離を求める
		Vector3 distance = posA - posB;
		//弾と弾の交差判定
		if ()
	}

	#pragma endregion
}
