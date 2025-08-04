#pragma once
#include "object.h"

class Player : Object {
public:
	Player(Game* game);
	~Player();

	void handleEvent(SDL_Event event);
	void update();
	void draw();
private:
	SDL_Texture* texture;
	SDL_FRect hitbox;
	Mix_Chunk* blips[4];

	int direction;
};

