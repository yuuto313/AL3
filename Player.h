#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include "DebugCamera.h"
#include "MyMath.h"

#include "BaseCharacter.h"

#pragma once
/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter{
public:

	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(const std::vector<Model*>&models) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;
	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();
	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Movement();
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

private:
	//ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;
	WorldTransform worldTransformWeapon_;

	//モデル
	/*Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterLeftArm_ = nullptr;
	Model* modelFighterRightArm_ = nullptr;*/

	//カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;


	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	
	uint16_t floatingCycle_ = 60;
	float tempFloat_ = 0.0f;

	float step_ = 0.0f;

	float amplitude_ = 0.5f;

	// キーボード入力
	Input* input_ = nullptr;
};
