#pragma once
#include "DirectXCommon.h"
#include "Sprite.h"
/// <summary>
/// フェード
/// </summary>
class Fade {
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialzie();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList);

private:
	Sprite* sprite_ = nullptr;
};
