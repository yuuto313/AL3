#include "BaseEnemyState.h"
#include <cmath>
#include <iostream>

BaseEnemyState::~BaseEnemyState() {}

void BaseEnemyState::DebugLog() { 
	std::cout << name_ << std::endl;
}

EnemyStateApproach::EnemyStateApproach(Enemy* enemy) : BaseEnemyState("State Approach", enemy) {}


EnemyStateLeave::EnemyStateLeave(Enemy* enemy) : BaseEnemyState("State Leave", enemy) {}
