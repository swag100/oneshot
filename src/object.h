#pragma once

#include <SDL.h>
class Game;

class Object{
public:
	Object(Game* game) : game(game) {}
	virtual ~Object() {};

	virtual void handleEvent(SDL_Event event) {}
	virtual void update() {}
	virtual void draw() {}

	Game* game;
};

