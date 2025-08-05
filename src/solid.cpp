#include <iostream>

#include "solid.h"
#include "game.h"

Solid::Solid(Game* game, int x, int y, int w, int h) : Object(game) {
	SDL_FRect initRect{ (float)x, (float)y, (float)w, (float)h };
	hitbox = initRect;
}

std::vector<SDL_FRect> Solid::getCollisions() {
	std::vector<SDL_FRect> collisions = {};
	for (const auto& objPtr : game->gameObjects) {
		if (objPtr) {
			// Check if objPtr points to a solid object
			if (Solid* solidPtr = dynamic_cast<Solid*>(objPtr.get())) {
				SDL_FRect theirRect = solidPtr->hitbox;

				if (solidPtr != this && this->checkForOverlap(theirRect, hitbox) && solidPtr->collidable) {
					collisions.push_back(theirRect);
				}
			}
		}
	}
	return collisions;
}
bool Solid::checkForOverlap(SDL_FRect a, SDL_FRect b) {
	return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}
void Solid::pushX() {
	std::vector<SDL_FRect> collisions = getCollisions();

	for (const SDL_FRect collision : collisions) {
		if (xVelocity != 0) {
			if (xVelocity > 0) {
				hitbox.x = collision.x - hitbox.w;
			}
			else {
				hitbox.x = collision.x + collision.w;
			}
		}
	}
}
void Solid::pushY() {
	std::vector<SDL_FRect> collisions = getCollisions();

	for (const SDL_FRect collision : collisions) {
		if (yVelocity != 0) {
			if (yVelocity > 0) {
				hitbox.y = collision.y - hitbox.h;
			}
			else {
				hitbox.y = collision.y + collision.h;
			}
		}
	}
}

void Solid::handleEvent(SDL_Event event) {}

void Solid::update() {
	//apply velocity!
	hitbox.x += (xVelocity) * game->deltaTime;
	pushX();

	hitbox.y += (yVelocity) * game->deltaTime;
	pushY();
}

void Solid::draw() {
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);

	SDL_Rect hitboxRectInt = { 
		(int)hitbox.x, (int)hitbox.y, 
		(int)hitbox.w, (int)hitbox.h 
	};
	SDL_RenderFillRect(game->renderer, &hitboxRectInt); // Draw a filled rectangle
}