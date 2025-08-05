#pragma once

#include "solid.h"

class Player : public Solid {
public:
	Player(Game* game);
	Player(Game* game, int x, int y);
	~Player();

	void init();

	void handleEvent(SDL_Event event);
	void update();
	void draw();
private:
	SDL_Texture* texture;
	Mix_Chunk* blips[4];

	int direction;
};

