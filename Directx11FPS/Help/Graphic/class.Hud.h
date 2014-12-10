#pragma once
#include "../../Global.h"
#include <d3d11.h>
#include <d3dx11tex.h>

/*
	As already mentioned, use D3D10_PRIMITIVE_TOPOLOGY_LINELIST or D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP to draw line primitives. 
	These will be 3D lines. To draw 2D lines, give them all the same fixed Z coordinate. 
	Make sure that this Z coordinate is inside your view frustum
*/

/*
	Remember that different users will run your application in different resolutions. 
	One size font is not going to be clearly readable on all resolutions. 
	So you may want to make 3-4 different font sizes and use certain ones for certain resolutions to fix this problem. 
*/



class Hud {	
public :
	// TaskHud Zobraz obrazok na 1 sec, potom pomaly z tmavni ( alpha-- ) a potom prec
};




