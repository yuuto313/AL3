#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Enemy : public BaseCharacter{
public:
	void Initialize(const std::vector<Model*>&models) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void Movement();

	/// <summary>
	/// 敵のWorldTransformを取得する
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform(){ return worldTransform_; }

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition()const;

	/// <summary>
	/// 攻撃を受けた時のリアクション
	/// </summary>
	void Reaction();

private:
	WorldTransform worldTransformWeapon_;
};
