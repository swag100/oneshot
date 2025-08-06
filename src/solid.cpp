#include <iostream>

#include "solid.h"
#include "game.h"
#include "constants.h"

Solid::Solid(Game* game) : Object(game) {}

Solid::Solid(Game* game, int x, int y, int w, int h) : Object(game) {
	SDL_FRect initRect{ (float)x, (float)y, (float)w, (float)h };
	hitbox = initRect;
} 

std::vector<Solid*> Solid::getCollisions() {
	std::vector<Solid*> collisions = {};
	for (const auto& objPtr : game->gameObjects) {
		// Check if objPtr points to a solid object
		if (Solid* solidPtr = dynamic_cast<Solid*>(objPtr.get())) {
			if (solidPtr != this && this->checkForOverlap(solidPtr->hitbox, hitbox) && solidPtr->collidable) {
				collisions.push_back(solidPtr);
			}
		}
	}
	return collisions;
}
bool Solid::checkForOverlap(SDL_FRect a, SDL_FRect b) {
	return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}
void Solid::pushX() {
	std::vector<Solid*> collisions = getCollisions();

	for (Solid* collision : collisions) {
		SDL_FRect theirBox = collision->hitbox;
		if (xVelocity != 0) {
			if (xVelocity > 0) {
				hitbox.x = theirBox.x - hitbox.w;
			}
			else {
				hitbox.x = theirBox.x + theirBox.w;
			}
			xVelocity = 0;
		}
	}
}
void Solid::pushY() {
	std::vector<Solid*> collisions = getCollisions();

	platform = NULL;
	for (Solid* collision : collisions) {
		SDL_FRect theirBox = collision->hitbox;
		if (yVelocity != 0) {
			if (yVelocity > 0) {
				hitbox.y = theirBox.y - hitbox.h;
				platform = collision;
				landed();
			}
			else {
				hitbox.y = theirBox.y + theirBox.h;
			}
			yVelocity = 0;
		}
	}
}
void Solid::applyGravity() {
	yVelocity += (yVelocity > 0 ? constants::FALL_GRAVITY : constants::GRAVITY) * game->deltaTime;
}

void Solid::handleEvent(SDL_Event event) {}

void Solid::update() {
	//add gravity
	if (!anchored) {
		applyGravity();
	}

	if (yVelocity > maxVelocity) {
		yVelocity = maxVelocity;
	}
	
	//apply velocity!
	hitbox.x += (xVelocity) * game->deltaTime;
	if (platform != NULL) {
		hitbox.x += platform->xVelocity * game->deltaTime;
	}
	pushX();

	hitbox.y += (yVelocity) * game->deltaTime;
	pushY();

	//REMOVE LATER
	if (hitbox.y > constants::SCREEN_HEIGHT) {
		hitbox.y = -hitbox.h;
	}
}

void Solid::draw() {
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);

	SDL_Rect hitboxRectInt = { 
		(int)hitbox.x, (int)hitbox.y, 
		(int)hitbox.w, (int)hitbox.h 
	};
	SDL_RenderFillRect(game->renderer, &hitboxRectInt); // Draw a filled rectangle
}