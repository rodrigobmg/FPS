#pragma once
#include "../../Help/Logic/Map/TerrainData.h"










//

// TILE ako VRCHOL z grafu pre AI nie mapu


//










namespace Path {
	
	struct Tile {
		TERRAIN_TILE* tile;
		UINT score;
		// co dalej treba ...
	};


}
