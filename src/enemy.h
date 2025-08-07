#pragma once

#include "solid.h"
#include <cmath>

// CREATE ENEMY.CPP FOR IMPLEMENTATIONS.

class Enemy : public Solid {
public:
	Enemy(Game* game, SDL_Point spawnPosition) : Solid(game, spawnPosition.x, spawnPosition.y, 16, 16) {}
	~Enemy() {}

	void handleEvent(SDL_Event event) {}
	void update() {
		xVelocity = std::sin((SDL_GetTicks() / 1000) * 20) * 64;
		//collide
		Solid::update();
	}
	void draw() {
		//call super
		Solid::draw();
	}
};

