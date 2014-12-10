#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//+ Struktura pre obrazovku / okno
struct Screen 
{
	UINT width;
	UINT height;
	bool fullscreen;

	Screen() {
		// Defaultne nestavenie pri spusteni
		fullscreen = false;
		width = 800;
		height = 600;
	}
};