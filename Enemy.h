#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <list>

#include "EnemyCanon.h"

class Player;

class Enemy : public BaseCharacter{
public:
	Enemy();
	~Enemy();

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
	/// 通常攻撃
	/// </summary>
	void Attack();

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
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// シリアルナンバーを取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetSerialNumber() const { return serialNumber_; }

	void SetPlayer(Player* player) { player_ = player; }

private:
	WorldTransform worldTransformWeapon_;

	//シリアルナンバー
	uint32_t serialNumber_ = 0;

	//次のシリアルナンバー
	static uint32_t nextSerialNumber_;

	std::list<EnemyCanon*> canons_;
	float coolTime_ = 3.0f;

	// 自キャラ
	Player* player_ = nullptr;

};
