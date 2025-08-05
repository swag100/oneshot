#pragma once

#include <vector>
#include "object.h"

//an object with a hitbox.

class Solid : public Object {
public:
	Solid(Game* game, int x, int y, int w, int h);

	std::vector<SDL_FRect> getCollisions();
	bool checkForOverlap(SDL_FRect a, SDL_FRect b);
	void pushX();
	void pushY();

	void handleEvent(SDL_Event event);
	void update();
	void draw();

	SDL_FRect hitbox;

	bool collidable = true; // will we push out of it?

	float xVelocity = 0;
	float yVelocity = 0;
};

