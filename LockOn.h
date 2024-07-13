#pragma once
#include "Sprite.h"
#include "Enemy.h"
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

private:
	uint32_t textureHandle_ = 0u;
	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;

	//ロックオン対象
	const Enemy* target_ = nullptr;
};
