#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

#pragma once
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 position, Vector3 rotate);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//カメラの移動スピード
	static inline const Vector3 kSpeed = {0.0f, 0.0f, -0.05f};
	//カメラの回転
	static inline const Vector3 kRotate = {0.0f, 0.0f, 0.0f};
};
