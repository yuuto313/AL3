#pragma once
#include "Sprite.h"
#include "Enemy.h"
#include "ViewProjection.h"
#include "Input.h"

#include <memory>


/// <summary>
/// ロックオン
/// </summary>
class LockOn {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="viewProjection"></param>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies,const ViewProjection& viewProjection);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ターゲット検索
	/// </summary>
	void Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	/// <summary>
	/// ワールドからスクリーンに座標変換
	/// </summary>
	Vector3 WorldToScreen(Vector3& worldPosition,const ViewProjection& viewprojection);
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
	Vector3 GetTargetPosition()const;
	/// <summary>
	/// ロックオン中かどうか確認
	/// </summary>
	/// <returns></returns>
	bool ExistTarget() const { return target_ ? true : false; }

private:
	uint32_t textureHandle_ = 0u;
	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;

	//ロックオン対象
	const Enemy* target_ = nullptr;

	//最小距離
	float minDistance_ = 10.0f;
	//最大距離
	float maxDistance_ = 100.0f;
	//角度範囲
	float angleRange_ = 20.0f * (float(M_PI) / 180.0f);

	Input* input_ = nullptr;

};
