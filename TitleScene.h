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
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return finished_; }

private:
	//終了フラグ
	bool finished_ = false;

	DirectXCommon* dxCommon_ = nullptr;

	Model* modelTitle_ = nullptr;
	Model* modelUI_ = nullptr;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformUI_;
	ViewProjection viewProjection_;

	// 存続時間(消滅までの時間)<秒>
	static inline const float kDuration = 3.0f;

	// 経過時間カウント
	float counter_ = 0.0f;
	// 増加フラグ
	bool isIncreasing_ = true; 

	// 色変更オブジェクト
	ObjectColor objectColor_;
	// 色の数値
	Vector4 color_;
};
