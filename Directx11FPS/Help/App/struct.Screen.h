#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct Screen {
	UINT width;
	UINT height;
	bool fullscreen;

	Screen() {
		fullscreen = false;
		width = 800;
		height = 600;
	}
};