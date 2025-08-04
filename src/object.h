#pragma once

#include <SDL_events.h>
class Game;

class Object{
public:
	Object(Game* game) : game(game) {}
	~Object() {}

	void handleEvent(SDL_Event event);
	void update();
	void draw();

	Game* game;
};

