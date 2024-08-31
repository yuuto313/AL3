#include <optional>
#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include "DebugCamera.h"
#include "MyMath.h"

#include "Hammer.h"
#include "BaseCharacter.h"
#include "GlobalVariables.h"


#pragma once

class LockOn;

enum class Behavior {
	kRoot,   // 通常状態
	kAttack, // 攻撃中
	kJump,//ジャンプ中
	kDash,//ダッシュ中
};

struct WorkDash {
	//ダッシュ用の媒介変数
	uint32_t dashParameter_ = 0;
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
	/// ジャンプ行動初期化
	/// </summary>
	void BehaviorJumpInitialize();

	/// <summary>
	/// ダッシュ行動初期化
	/// </summary>
	void BehaviorDashInitialize();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// ジャンプ行動更新
	/// </summary>
	void BehaviorjumpUpdate();

	/// <summary>
	/// ダッシュ行動更新
	/// </summary>
	void BehaviorDashUpdate();

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
	/// 振る舞いを設定
	/// </summary>
	/// <param name="behavior"></param>
	void SetBehavior(Behavior behavior) { behaviorRequest_ = behavior; }
	/// <summary>
	/// ロックオンセッター
	/// </summary>
	/// <param name="lockOn"></param>
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const override;
	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;
	/// <summary>
	/// ハンマーのゲッター
	/// </summary>
	/// <returns></returns>
	Hammer* GetHammer() { return hammer_.get(); }

private:

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	// 振る舞いのリクエスト
	// std::nulloptはそのstd::optionalが無効状態であることを表す値
	// std::nulloptではなくBehavior型の値を入れたときは有効状態となる
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// ロックオン
	const LockOn* lockOn_ = nullptr;

	// ハンマー(武器)
	std::unique_ptr<Hammer> hammer_;

	// キーボード入力
	Input* input_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;

	//カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;


	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	
	int32_t floatingCycle_ = 60;
	float tempFloat_ = 0.0f;

	float step_ = 0.0f;

	float amplitude_ = 0.5f;

	//速度
	Vector3 velocity_ = {};

	//目標角度
	float targetAngle_ = 0.0f;

	WorkDash workDash_;
};
