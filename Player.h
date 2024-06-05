#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"

#pragma once
/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,uint32_t reticleTextureHandle, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 旋回（回転）
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// レティクルのワールド座標を取得
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <returns></returns>
	Vector3 GetReticleWorldPosition(WorldTransform& worldTransform);

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 弾リストを取得
	/// </summary>
	/// <returns></returns>
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	/// <summary>
	/// 半径を取得するゲッター
	/// </summary>
	/// <param name="radius"></param>
	/// <returns></returns>
	float GetRadius() {return kRadius; }

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// レティクルの更新処理
	/// </summary>
	/// <param name="viewProjection"></param>
	void Reticle(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// レティクルのテクスチャハンドル
	uint32_t textureHandleReticle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;

	//半径
	static inline const float kRadius = 1.0f;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3Dreticle_;

	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
};
