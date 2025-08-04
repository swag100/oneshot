#pragma once
#include <SDL.h>
#include "game.h"

class Player {
public:
	Player(Game* game);
	~Player();

	void handleEvent(SDL_Event event);
	void update();
	void draw();

private:
	Game* game;

	SDL_Texture* texture;
	SDL_FRect hitbox;

	int direction;

};

