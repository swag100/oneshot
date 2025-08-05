#pragma once

#include <SDL.h>
class Game;

class Object{
public:
	Object(Game* game) : game(game) {}
	virtual ~Object() {};

	virtual void handleEvent(SDL_Event event) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;

	Game* game;
};

