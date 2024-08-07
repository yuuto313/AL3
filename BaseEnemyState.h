#pragma once
#include <string>
class Enemy;

/// <summary>
/// 敵の状態基底
/// </summary>
class BaseEnemyState {
public:
	BaseEnemyState(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy){};
	virtual ~BaseEnemyState();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// デバッグログ出力
	/// </summary>
	virtual void DebugLog();

protected:
	//状態名
	std::string name_;
	//捜査対象の敵
	Enemy* enemy_ = nullptr;
};

class EnemyStateApproach : public BaseEnemyState{
public:
	EnemyStateApproach(Enemy* enemy);
	void Update();
};

class EnemyStateLeave : public BaseEnemyState {
public:
	EnemyStateLeave(Enemy* enemy);
	void Update();
};