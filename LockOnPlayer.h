#pragma once
#include "Sprite.h"
#include "Player.h"

/// <summary>
/// プレイヤーをロックオン
/// </summary>
class LockOnPlayer {
public:

	/// <summary>
	///	初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player"></param>
	/// <param name="viewProjection"></param>
	void Update(const std::unique_ptr<Player>& player, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	///	ロックオン対象の検索
	/// </summary>
	/// <param name="player"></param>
	/// <param name="viewProjection"></param>
	void Search(const std::unique_ptr<Player>& player, const ViewProjection& viewProjection);

	/// <summary>
	/// ワールドからスクリーンに座標変換
	/// </summary>
	Vector3 WorldToScreen(Vector3& worldPosition, const ViewProjection& viewprojection);

	/// <summary>
	/// 範囲外選択判定
	/// </summary>
	/// <param name="viewprojection"></param>
	/// <returns></returns>
	bool OutsideSelectionRange(const ViewProjection& viewProjection);

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition() const;

	/// <summary>
	/// ロックオン中かどうか確認
	/// </summary>
	/// <returns></returns>
	bool ExistTarget() const { return target_ ? true : false; }

private:
	// テクスチャ
	uint32_t textureHandel_ = 0u;

	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnSprite_;


	// ロックオン対象
	const Player* target_ = nullptr;


	// 最小距離
	float minDistance_ = 10.0f;
	// 最大距離
	float maxDistance_ = 30.0f;
	// 角度範囲
	float angleRange_ = 20.0f * (float(M_PI) / 180.0f);


};
