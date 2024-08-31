#include "DeathParticles.h"
#include <cassert>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, Player* player) { 
	assert(model);
	
	model_ = model;
	viewProjection_ = viewProjection;

}

void DeathParticles::Update() {}

void DeathParticles::Draw() {}
