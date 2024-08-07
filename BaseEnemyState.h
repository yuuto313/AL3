#pragma once

class Enemy;

class BaseEnemyState {
public:
	virtual ~BaseEnemyState();
	virtual void Update(Enemy* enemy) = 0;

protected:
	//Enemy* enemy_ = nullptr;
};

class EnemyStateApproach : public BaseEnemyState{
public:
	void Update(Enemy* enemy);
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update(Enemy* enemy);
};