#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Enemy : public BaseCharacter{
public:
	Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	void Initialize(const std::vector<Model*>&models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 動き
	/// </summary>
	void Movement();

	/// <summary>
	/// 攻撃を受けた時のリアクション
	/// </summary>
	void Reaction();

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
	/// シリアルナンバーを取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetSerialNumber() const { return serialNumber_; }

private:
	WorldTransform worldTransformWeapon_;

	//シリアルナンバー
	uint32_t serialNumber_ = 0;

	//次のシリアルナンバー
	static uint32_t nextSerialNumber_;
};
