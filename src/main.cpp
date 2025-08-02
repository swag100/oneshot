#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// You must include the command line parameters for your main function to be recognized by SDL
int main(int argc, char** args) {

	// Pointers to our window and surface
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Initialize SDL. SDL_Init will return -1 if it fails.
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)){

		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		system("pause");
		return 1;
	}
	if (TTF_Init() < 0) {
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
		system("pause");
		return 1;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		system("pause");
		return 1;
	}

	// Create our window

	int result = SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer);
	if (result != 0) {
		std::cout << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
	}

	SDL_Texture* screen = SDL_CreateTexture(renderer, 
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		SCREEN_WIDTH, SCREEN_HEIGHT
	);

	//THIS CREATES MIXELS AAAAAAAHHHH
	//SDL_RenderSetLogicalSize(renderer, 320, 240);

	//test fonts

	TTF_Font* font;
	font = TTF_OpenFont("res/font/Zont.ttf", 24);
	if (!font) {
		std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
	}
	SDL_Surface* testTextSurf;
	SDL_Color color = { 255, 255, 255 };
	testTextSurf = TTF_RenderText_Solid(font, "Hello World!", color);
	if (!testTextSurf) {
		std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
	}
	SDL_Texture* text_texture;
	text_texture = SDL_CreateTextureFromSurface(renderer, testTextSurf);

	//player image

	SDL_Surface* image = IMG_Load("res/images/tn_flag.png");

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, 128);

	//get info about it
	int w, h, access;
	unsigned int format;
	SDL_QueryTexture(texture, &format, &access, &w, &h);

	int playerRotation = 0;

	SDL_FreeSurface(image);
	image = NULL;

	//SFX
	Mix_Chunk* blips[4];
	for (int i= 0; i < 4; ++i) {
		std::string filename = "res/sounds/blip" + std::to_string(i + 1) + ".wav";
		blips[i] = Mix_LoadWAV(filename.c_str());
		if (!blips[i]) {
			std::cout << "Failed to load sound effect: " << Mix_GetError() << std::endl;
			system("pause");
			return 1;
		}
	}

	// Get the surface from the window
	//winSurface = SDL_GetWindowSurface(window);

	/*/ Make sure getting the surface succeeded
	if (!winSurface) {
		std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
		return 1;
	}*/

	SDL_Event event;
	bool running = true;

	SDL_Rect player = {100, 100, 64, 64};
	player.x = 100; // Set the x position of the player

	// Main loop
	while (running) {
		// Event loop
		SDL_SetRenderTarget(renderer, screen);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		// Fill the window with a white rectangle
		SDL_RenderClear(renderer);
		//SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 0, 0, 0));

		while ( SDL_PollEvent( &event ) ) {
			// check event type
			switch ( event.type ) {
				case SDL_QUIT:
					// shut down
					running = false;

					break;
				case SDL_KEYDOWN:
					// test keycode
					switch ( event.key.keysym.sym ) {
						case SDLK_LEFT:
							std::cout << "A key pressed" << std::endl;
							playerRotation -= 10; // Move player up
							Mix_PlayChannel(-1, blips[0], 0); // Play sound effect
							break;
						case SDLK_RIGHT:
							std::cout << "D key pressed" << std::endl;
							Mix_PlayChannel(-1, blips[1], 0); // Play sound effect
							playerRotation += 10; // Move player up
							break;
						case SDLK_UP:
							std::cout << "W key pressed" << std::endl;
							player.y -= 10; // Move player up
							Mix_PlayChannel(-1, blips[2], 0); // Play sound effect
							break;
						case SDLK_DOWN:
							std::cout << "S key pressed" << std::endl;
							player.y += 10; // Move player up
							Mix_PlayChannel(-1, blips[3], 0); // Play sound effect
							break;
						// etc
					}

					break;
			}
		}

		// Fill the window with a white rectangle
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &player); // Draw a filled rectangle

		SDL_Rect newRect = { player.x - w + (player.w / 2), player.y - h + (player.h / 2), w * 2, h * 2 };

		SDL_Point rotPoint;
		rotPoint.x = w;
		rotPoint.y = h;

		SDL_RenderCopyEx(renderer, texture, NULL, &newRect, (playerRotation % 360), &rotPoint, SDL_FLIP_HORIZONTAL);
		//SDL_RenderCopy(renderer, texture, NULL, &newRect);
		//SDL_FillRect(winSurface, &player, SDL_MapRGB(winSurface->format, 0, 255, 0));

		SDL_Rect dest = { 0, 0, testTextSurf->w, testTextSurf->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &dest);

		//render target to window
		SDL_SetRenderTarget(renderer, NULL);

		//

		SDL_RenderCopy(renderer, screen, NULL, NULL);

		// Update window
		SDL_RenderPresent(renderer);

		// Wait before next frame
		SDL_Delay(15);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_FreeSurface(testTextSurf);
	SDL_DestroyTexture(text_texture);
	SDL_DestroyTexture(screen);
	screen = NULL;
	SDL_DestroyTexture(texture);
	texture = NULL;

	renderer = NULL;
	window = NULL;

	for (int i = 0; i < 4; ++i) {
		if (blips[i]) {
			Mix_FreeChunk(blips[i]);
			blips[i] = NULL;
		}
	}

	// Quit SDL
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	// End the program
	return 0;
}