#pragma once
#include "Collider.h"
#include <Model.h>
#include <WorldTransform.h>

/// <summary>
/// ハンマー
/// </summary>
class Hammer final : public Collider{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

	const Vector3& GetRotation() const { return worldTransform_.rotation_;}
	void SetRotation(const Vector3& rotaion) { worldTransform_.rotation_ = rotaion; }

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	float currentRotationAngleX = 0.0f;
	const float rotationSpeed = (float)M_PI / 45.0f;
	const float targetRotationAngleX = (float)M_PI / 2.0f;
};
