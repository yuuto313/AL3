#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include "DebugCamera.h"
#include "MyMath.h"
#include <optional>

#include "BaseCharacter.h"
#include "GlobalVariables.h"

#pragma once

class LockOn;

enum class Behavior {
	kRoot,   // 通常状態
	kAttack, // 攻撃中
	kJump,//ジャンプ中
};

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
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();
	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();
	/// <summary>
	/// 攻撃行動初期化
	/// </summary>
	void BehaviorAttackInitialize();
	/// <summary>
	/// ジャンプ行動初期化
	/// </summary>
	void BehaviorJumpInitialize();
	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();
	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();
	/// <summary>
	/// ジャンプ行動更新
	/// </summary>
	void BehaviorjumpUpdate();
	/// <summary>
	///	Behavior遷移の実装
	/// </summary>
	void ChangeBehavior();
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
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	/// <summary>
	/// 自キャラのWorldTransformを取得する
	/// </summary>
	/// <returns></returns>
	const ViewProjection* GetViewProjection() { return cameraViewProjection_; }
	/// <summary>
	/// ロックオンセッター
	/// </summary>
	/// <param name="lockOn"></param>
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

private:
	//ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;
	WorldTransform worldTransformWeapon_;

	//カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;


	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	
	int32_t floatingCycle_ = 60;
	float tempFloat_ = 0.0f;

	float step_ = 0.0f;

	float amplitude_ = 0.5f;

	float currentRotationAngleX = 0.0f;
	const float rotationSpeed = (float)M_PI / 45.0f;
	const float targetRotationAngleX = (float)M_PI / 2.0f;

	//速度
	Vector3 velocity_ = {};

	//振る舞い
	Behavior behavior_ = Behavior::kRoot;
	//振る舞いのリクエスト
	//std::nulloptはそのstd::optionalが無効状態であることを表す値
	//std::nulloptではなくBehavior型の値を入れたときは有効状態となる
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//ロックオン
	const LockOn* lockOn_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

};
