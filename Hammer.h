#pragma once
#include "Collider.h"
#include <Model.h>
#include <WorldTransform.h>

class Player;

/// <summary>
/// ハンマー
/// </summary>
class Hammer final : public Collider{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="player"></param>
	void Initialize(Model* model,Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

	const Vector3& GetRotation() const { return worldTransform_.rotation_;}
	void SetRotation(const Vector3& rotaion) { worldTransform_.rotation_ = rotaion; }

	/// <summary>
	/// 親子関係を結ぶ
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	Player* player_ = nullptr;

	float currentRotationAngleX = 0.0f;
	const float rotationSpeed = (float)M_PI / 45.0f;
	const float targetRotationAngleX = (float)M_PI / 2.0f;
};
