#include "Model.h"
#include "WorldTransform.h"
#include "cassert"

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	Skydome();
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Model* model,ViewProjection* viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* model_ = nullptr;
};