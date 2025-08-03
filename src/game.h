#pragma once
#include <SDL.h>

class game
{
public:
	game(const char* title, int width, int height);
	~game();

	int init();

	void handleEvents();
	void update();
	void draw();

	void run();

private:
	bool done;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* screen;
};

