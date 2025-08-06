#pragma once

namespace constants {
	inline constexpr int SCREEN_WIDTH = 320;
	inline constexpr int SCREEN_HEIGHT = 240;

	inline constexpr int WINDOW_WIDTH = 640;
	inline constexpr int WINDOW_HEIGHT = 480;

	inline const char* WINDOW_TITLE = "deltarune2";
	inline constexpr int WINDOW_FLAGS = SDL_WINDOW_RESIZABLE; // NULL for none

	inline constexpr int FRAME_RATE = 60;
	inline const float FRAME_TIME = 1000 / FRAME_RATE;
	inline const float MAX_FRAME_TIME = 0;

	inline constexpr int GRAVITY = 300;
	inline constexpr int FALL_GRAVITY = 1000;
}
