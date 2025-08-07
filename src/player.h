#pragma once

#include "solid.h"

class Player : public Solid {
public:
	Player(Game* game);
	Player(Game* game, SDL_Point spawnPosition);
	~Player();

	void init();
	void jump();

	int getGravity(); // override solid
	void landed();

	void handleEvent(SDL_Event event);
	void update();
	void draw();
private:
	SDL_Texture* texture;
	Mix_Chunk* blips[4];

	int direction;

	int maxJumpHeight = 180;
	int minJumpHeight = 320;
	int runSpeed = 16;

	float xInertia = 0; // applied with no damping

	bool hasJumped = false;
};

