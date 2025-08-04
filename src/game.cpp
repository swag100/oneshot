#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

#include "game.h"
#include "constants.h"
#include "player.h"

//maybe move to util namespace?
//you'd have to add game as a param, but im too lazy to figure that out.

//If the condition is met, quit the game and print the error.
void Game::quitIfError(bool errorCond) {
	if (errorCond) {
		std::cout << SDL_GetError() << std::endl;
		system("pause");
		done = true;
	}
}

Game::Game() {
	//INIT -- initialize all SDL libs + window and renderer, and make sure they don't error out. 

	quitIfError(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG));
	quitIfError(TTF_Init() < 0);
	quitIfError(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0);
	quitIfError(SDL_Init(SDL_INIT_EVERYTHING) < 0);

	window = SDL_CreateWindow(
		constants::WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		constants::WINDOW_WIDTH,
		constants::WINDOW_HEIGHT, 
		constants::WINDOW_FLAGS
	);
	quitIfError(window == NULL);
	SDL_SetWindowMinimumSize(
		window, 
		constants::SCREEN_WIDTH, 
		constants::SCREEN_HEIGHT
	);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	quitIfError(renderer == NULL);

	screen = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_TARGET,
		constants::SCREEN_WIDTH, 
		constants::SCREEN_HEIGHT
	);

	//remove later
	player = new Player{ this };

	run();
}

Game::~Game() {
	//CLEANUP -- quit sdl, destroy window, renderer, textures, etc.

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_DestroyTexture(screen);
	screen = NULL;

	//possibly loop through all textures and clean em up
	//loop through all objects and delete em
	delete player;

	// Quit SDL
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

void Game::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			done = true;
		}
		player->handleEvent(e);
	}
}

void Game::update() {
	player->update();
	std::cout << deltaTime << std::endl;
}

void Game::draw() {
	// Set render target to screen texture and clear it
	SDL_SetRenderTarget(renderer, screen);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// Okay let's actually draw now
	player->draw();
	

	// Set render target to window and present our awesome drawing
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, screen, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Game::run() {
	while (!done) {
		startTime = SDL_GetTicks();
		
		// Update
		handleEvents();
		update();
		draw();

		computeTime = SDL_GetTicks() - startTime;
		if (computeTime < constants::FRAME_TIME) {
			SDL_Delay(constants::FRAME_TIME - computeTime);
		}
		deltaTime = (float)(SDL_GetTicks() - startTime) / 1000.f;
	}
}