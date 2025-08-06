#pragma once

#include "solid.h"
#include <cmath>

//COMPLETELY INLINE BECAUSE IT WAS MADE QUICKLY. RENAME THIS FILE TO ENEMY AND CREATE ENEMY.CPP FOR IMPLEMENTATIONS.

class Lil : public Solid {
public:
	Lil(Game* game, SDL_Point spawnPosition) : Solid(game, spawnPosition.x, spawnPosition.y, 16, 16) {}
	~Lil() {}

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

