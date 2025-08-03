#include "game.h"

#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "constants.h"

game::game(const char* title, int width, int height) {
	if (init() != 0) {
		done = true;
	}
}
game::~game() {
	//CLEANUP -- quit sdl, destroy window, renderer, textures, etc.

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_DestroyTexture(screen);
	screen = NULL;

	//possibly loop through all textures and clean em up

	// Quit SDL
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

int game::init() {
	//INIT -- initialize all SDL libs + window and renderer, and make sure they don't error out. 

	//TODO -- clean this up, they all have the same body. Really it's just finding a good name for the function.
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << IMG_GetError() << std::endl;
		system("pause");
		return 1;
	}
	if (TTF_Init() < 0) {
		std::cout << TTF_GetError() << std::endl;
		system("pause");
		return 1;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << Mix_GetError() << std::endl;
		system("pause");
		return 1;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	// Create our window
	int result = SDL_CreateWindowAndRenderer(
		constants::WINDOW_WIDTH, 
		constants::WINDOW_HEIGHT, 
		constants::WINDOW_FLAGS, 
		&window, 
		&renderer
	);
	if (result < 0) {
		std::cout << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	// Create a texture for pixel accuracy
	screen = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_TARGET,
		constants::SCREEN_WIDTH, 
		constants::SCREEN_HEIGHT
	);
}

void game::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			done = true;
		}
	}
}
void game::update() {

}
void game::draw() {

}

void game::run() {
	while (!done) {
		handleEvents();
		update();
		draw();
	}
}