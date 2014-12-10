#pragma once
#include <DXGIType.h>
#include "../App/Base/struct.Screen.h"

// namespace App by malo byt
struct GraphicOptions {
	struct Fov {
		float neardepth;
		float fardepth;
	}; 
	
	bool vsyn;
	Screen screen;
	DXGI_SAMPLE_DESC multysampling;
	Fov fov;
};

struct DefaultGraphicOptions : public GraphicOptions 
{
	DefaultGraphicOptions() {
		screen.width = 800;
		screen.height = 600;
		screen.fullscreen = false;
		
		vsyn = true;
		multysampling.Count = 1;
		multysampling.Quality = 0; // nefunguje a pada program
		fov.neardepth = 0.01f;
		fov.fardepth = 100000.f;
	}
};

// GraphicOptions mame premennu pre aktulne pouzity
// GraphicOptions mame premennu uzivatel co ma nastavene