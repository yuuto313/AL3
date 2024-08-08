#include "EnemyBullet.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"
#include <list>

//自機クラスの前方宣言
class Player;
#pragma once

/// <summary>
/// 敵
/// </summary>
class Enemy : public Collider{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& approachVelocity,const Vector3& leaveVelocity);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void InitializeApploach();

	/// <summary>
	/// 接近フェーズの更新関数
	/// </summary>
	void UpdateApploach();

	/// <summary>
	/// 離脱フェーズの更新関数
	/// </summary>
	void UpdateLeave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// PlayerはGameSceneがもっているので借りるためのセッターを作成
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; };

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// 弾リストを取得
	/// </summary>
	/// <returns></returns>
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	/// <summary>
	/// 半径を取得するゲッター
	/// </summary>
	/// <param name="radius"></param>
	/// <returns></returns>
	float GetRadius() { return kRadius; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルのポインタ
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 approachVelocity_;
	Vector3 leaveVelocity_;

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;

	//敵の弾
	std::list<EnemyBullet*> bullets_;

	//発射タイマー
	int32_t shotTimer_ = 0;

	//自キャラ
	Player* player_ = nullptr;

	static inline const float kRadius = 1.0f;


public:
	//発射間隔
	static const int kFireInterval = 60;

};
