#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FRAME_RATE 60

// You must include the command line parameters for your main function to be recognized by SDL
int SDL_main(int argc, char** args) {

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
	font = TTF_OpenFont("res/font/RpgSlant.ttf", 24); //16 is default
	if (!font) {
		std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
	}
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* testTextSurf;
	SDL_Texture* text_texture;

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

	SDL_FRect player = {100, 100, 64, 64};

	//fps
	Uint32 lastFpsCheckTime = SDL_GetTicks();
	double fps = 0.0;

	//DT
	const int desiredFrameTime = 1000 / FRAME_RATE; // Desired frame time in milliseconds
	int computeTime = 0;
	float deltaTime = 0;
	int startTime = 0;

	// Main loop
	while (running) {
		//deltaTime += desiredFrameTime - deltaTime; // Reset deltaTime to the desired frame time
		startTime = SDL_GetTicks(); // Get the current time in milliseconds

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
						case SDLK_SPACE:
							std::cout << "A key pressed" << std::endl;
							playerRotation -= 5; // Move player up
							Mix_PlayChannel(-1, blips[(rand() % 4)], 0); // Play sound effect
							break;
					}

					break;
			}
		}

		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		// Check if a specific key is pressed
		playerRotation += ((bool)keyboardState[SDL_SCANCODE_W] - (bool)keyboardState[SDL_SCANCODE_S]) * 600 * deltaTime; // Rotate player
		player.x += ((bool)keyboardState[SDL_SCANCODE_D] - (bool)keyboardState[SDL_SCANCODE_A]) * 300 * deltaTime;

		// Fill the window with a white rectangle
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_Rect playerRect = { (int)player.x, (int)player.y, (int)player.w, (int)player.h };
		SDL_RenderFillRect(renderer, &playerRect); // Draw a filled rectangle

		SDL_Rect newRect = { player.x - w + (player.w / 2), player.y - h + (player.h / 2), w * 2, h * 2 };

		SDL_Point rotPoint;
		rotPoint.x = w;
		rotPoint.y = h;

		SDL_RenderCopyEx(renderer, texture, NULL, &newRect, (playerRotation % 360), &rotPoint, SDL_FLIP_HORIZONTAL);
		//SDL_RenderCopy(renderer, texture, NULL, &newRect);
		//SDL_FillRect(winSurface, &player, SDL_MapRGB(winSurface->format, 0, 255, 0));


		//display fps every second
		Uint32 currentFpsCheckTime = SDL_GetTicks();
		if (currentFpsCheckTime - lastFpsCheckTime >= 1000) { // Check if 1 second (1000 ms) has passed
			fps = 1.f / deltaTime; // Calculate FPS
			// Perform actions that should happen every second
			// Example: printf("One second passed!\n");
			lastFpsCheckTime = currentFpsCheckTime; // Reset the last check time
		}

		testTextSurf = TTF_RenderText_Solid(font, std::to_string(fps).c_str(), color);
		if (!testTextSurf) {
			std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
		}
		text_texture = SDL_CreateTextureFromSurface(renderer, testTextSurf);
		SDL_Rect dest = { 0, 0, testTextSurf->w, testTextSurf->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &dest);
		SDL_FreeSurface(testTextSurf);
		SDL_DestroyTexture(text_texture);

		//render target to window
		SDL_SetRenderTarget(renderer, NULL);

		//

		SDL_RenderCopy(renderer, screen, NULL, NULL);

		// Update window
		SDL_RenderPresent(renderer);

		//SDL_Delay(16);
		computeTime = SDL_GetTicks() - startTime; // Calculate delta time in milliseconds
		if (computeTime < desiredFrameTime)
		{
			//Wait remaining time
			SDL_Delay(desiredFrameTime - computeTime);
		}
		// IMPORTANT: calculate deltaTime after the delay to account for the delay time.
		deltaTime = (float)(SDL_GetTicks() - startTime) / 1000.f;
		//std::cout << deltaTime << " ms" << std::endl;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

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