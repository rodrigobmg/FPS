#pragma once
#include "../Math/class.vec.h"
#include "../Shapes/Shapes.h"
using namespace Shape;
struct ID3D11Buffer;

struct TERRAIN_VERTEX
{
	vec pos;
	vec norm;
	vec2 uv;
};

struct GRASS_VERTEX
{
	vec pos;
	vec2 uv;
};

class TERRAIN_TILE
{
public:
	ID3D11Buffer* pVB11;
	unsigned int NumVertices;
	TERRAIN_VERTEX* pRawVertices;
	vec4 Color;
	AABBMM BBox;
};
