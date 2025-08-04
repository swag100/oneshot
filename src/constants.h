#pragma once

namespace constants {
	constexpr int SCREEN_WIDTH = 320;
	constexpr int SCREEN_HEIGHT = 240;

	constexpr int WINDOW_WIDTH = 640;
	constexpr int WINDOW_HEIGHT = 480;

	const char* WINDOW_TITLE = "deltarune2";
	constexpr int WINDOW_FLAGS = SDL_WINDOW_RESIZABLE; // NULL for none

	constexpr int FRAME_RATE = 60;
	constexpr float FRAME_TIME = 1000 / FRAME_RATE; 
}
