#pragma once

#include <SDL.h>
#include <memory>
#include <vector>
class Object;

class Game {
public:
	Game();
	~Game();

	void quitIfError(bool cond);

	void handleEvents();
	void update();
	void draw();

	void run();

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* screen; // Contains the game at its native resolution

	std::vector<std::unique_ptr<Object>> gameObjects;

	int computeTime = 0;
	float deltaTime = 0;
	int startTime = 0;

	bool done;

};