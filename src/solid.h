#pragma once

#include <vector>
#include "object.h"

//an object with a hitbox.

class Solid : public Object {
public:
	Solid(Game* game);
	Solid(Game* game, int x, int y, int w, int h);

	std::vector<Solid*> getCollisions();
	bool checkForOverlap(SDL_FRect& a, SDL_FRect& b);
	void pushX();
	void pushY();
	virtual void applyGravity();

	virtual void landed() {}

	void handleEvent(SDL_Event event);
	void update();
	void draw();

	SDL_FRect hitbox;

	bool collidable = true; // will we push out of it?
	bool anchored = false; // can it be moved?

	float xVelocity = 0;
	float yVelocity = 0;

	int maxVelocity = 800;

	Solid* platform; // pointer to what we're standing on
};

