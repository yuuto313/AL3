#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectXCommon.h"

#include "Fade.h"

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

	/// <summary>
	/// フェーズの切り替え
	/// </summary>
	void ChangePhase();


private:
	DirectXCommon* dxCommon_ = nullptr;

	Model* titleModel_ = nullptr;
	Model* UIModel_ = nullptr;
	WorldTransform titleWorldTransform_;
	WorldTransform UIWorldTransform_;
	ViewProjection viewProjection_;

	//終了フラグ
	bool finished_ = false;

	//フェード
	Fade* fade_ = nullptr;

	//シーンのフェーズ
	enum class Phase {
		//フェードイン
		kFadeIn,
		//メイン部
		kMain,
		//フェードアウト
		kFadeOut,
	};

	//現在のフェーズ
	Phase phase_ = Phase::kFadeIn;
};
