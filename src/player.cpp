#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

#include "player.h"
#include "game.h"

Player::Player(Game* game) : Solid(game, 0, 0, 16, 16) {
	init();
}
Player::Player(Game* game, int x, int y) : Solid(game, x, y, 16, 16) {
	init();
}
Player::~Player() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}

void Player::init() {

	SDL_Surface* tempSurface = IMG_Load("res/images/tn_flag.png");

	texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, 128);

	SDL_FreeSurface(tempSurface);
	tempSurface = NULL;

	for (int i = 0; i < 4; ++i) {
		std::string filename = "res/sounds/blip" + std::to_string(i + 1) + ".wav";
		blips[i] = Mix_LoadWAV(filename.c_str());
		game->quitIfError(!blips[i]);
	}
}

void Player::handleEvent(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_SPACE) {
			direction -= 5;
			Mix_PlayChannel(-1, blips[(rand() % 4)], 0);

			yVelocity -= 200;
		}
	}
}

void Player::update() {
	//game->deltaTime
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	xVelocity = (
		(bool)keyboardState[SDL_SCANCODE_D] -
		(bool)keyboardState[SDL_SCANCODE_A]
	) * 300;
	direction += (
		(bool)keyboardState[SDL_SCANCODE_E] -
		(bool)keyboardState[SDL_SCANCODE_Q]
	) * 600 * game->deltaTime;

	//add gravity
	yVelocity += 400 * game->deltaTime;

	//fix deltatime so resizing window doesn't make you fall through the ground

	//collide
	Solid::update();

}

void Player::draw() {
	SDL_Point size;
	SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
	SDL_Rect newRect = { 
		hitbox.x - size.x + (hitbox.w / 2), 
		hitbox.y - size.y + (hitbox.h / 2), 
		size.x * 2, 
		size.y * 2 
	};

	SDL_RenderCopyEx(game->renderer, texture, NULL, &newRect, (direction % 360), &size, SDL_FLIP_HORIZONTAL);

	//call super
	Solid::draw();
}