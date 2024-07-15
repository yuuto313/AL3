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

	Vector3 GetCenterPosition()const;

private:
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformWeapon_;
};
