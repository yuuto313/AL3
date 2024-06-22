#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include "DebugCamera.h"
#include "MyMath.h"

#pragma once
/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:

	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* modelFighterBody_, Model* modelFighterHead_, Model* modelFighterLightArm_, Model* modelFighterRightArm_, ViewProjection* viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 自キャラのWorldTransformを取得する
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }
	/// <summary>
	/// 自キャラのWorldTransformを取得する
	/// </summary>
	/// <returns></returns>
	const ViewProjection* GetViewProjection() { return cameraViewProjection_; }
	/// <summary>
	/// カメラのビュープロジェクションのみ参照するセッター
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) { cameraViewProjection_ = viewProjection;}
	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatingGimmick();
	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatingGimmick();

private:
	//ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;

	//モデル
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterLeftArm_ = nullptr;
	Model* modelFighterRightArm_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;
	//カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	// キーボード入力
	Input* input_ = nullptr;
};
