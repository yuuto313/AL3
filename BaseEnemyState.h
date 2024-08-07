#pragma once

class Enemy;

class BaseEnemyState {
public:
	virtual ~BaseEnemyState();
	virtual void Update() = 0;

protected:
	Enemy* enemy_ = nullptr;
};

class EnemyStateApproach : public BaseEnemyState{
public:
	void Update();
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update();
};