#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

#include "DirectXCommon.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了フラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return finished_; }

private:
	DirectXCommon* dxCommon_ = nullptr;

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//終了フラグ
	bool finished_ = false;
};
