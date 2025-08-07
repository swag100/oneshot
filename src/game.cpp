#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "game.h"
#include "constants.h"
#include "player.h"
#include "enemy.h"

//maybe move to util namespace?
//you'd have to add game as a param, but im too lazy to figure that out.

//If the condition is met, quit the game and print the error.
void Game::quitIfError(bool errorCond) {
	if (errorCond) {
		SDL_Log(SDL_GetError());
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

	SDL_Point lilPosition = { 100, 100 };
	gameObjects.push_back(std::make_unique<Enemy>(this, lilPosition));

	std::unique_ptr<Solid> ground = std::make_unique<Solid>(this);
	ground->hitbox = { 8, 200, 200, 32 };
	ground->anchored = true;
	gameObjects.push_back(std::move(ground));

	std::unique_ptr<Solid> ground2 = std::make_unique<Solid>(this);
	ground2->hitbox = { 200, 160, 90, 32 };
	ground2->anchored = true;
	gameObjects.push_back(std::move(ground2));

	std::unique_ptr<Solid> ground3 = std::make_unique<Solid>(this);
	ground3->hitbox = { -10, 80, 16, 80 };
	ground3->anchored = true;
	gameObjects.push_back(std::move(ground3));

	SDL_Point playerPosition = { 100, 100 };
	gameObjects.push_back(std::make_unique<Player>(this, playerPosition));

	run();
}

Game::~Game() {
	//CLEANUP -- quit sdl, destroy window, renderer, textures, etc.

	gameObjects.clear();

	SDL_DestroyTexture(screen);
	screen = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

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
		for (const auto& objPtr : gameObjects) {
			objPtr->handleEvent(e);
		}
	}
}

void Game::update() {
	for (const auto& objPtr : gameObjects) {
		objPtr->update();
	}
}

void Game::draw() {
	// Set render target to screen texture and clear it
	SDL_SetRenderTarget(renderer, screen);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// Okay let's actually draw now
	for (const auto& objPtr : gameObjects) {
		objPtr->draw();
	}

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
		if (constants::MAX_FRAME_TIME > constants::FRAME_TIME && deltaTime >= constants::MAX_FRAME_TIME) {
			deltaTime = constants::MAX_FRAME_TIME;
		}
	}
}