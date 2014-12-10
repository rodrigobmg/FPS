#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include "../../Graphic/Structures/inter.IBase.h"
#include "../../Math/class.vec.h"

namespace Map 
{

struct RastertekVertexType {
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

const int TEXTURE_REPEAT = 8;

class BaseTerrain : 
	public Structures::IBase<RastertekVertexType, unsigned long>
{
public:
	struct HeightMapType {
		vec position;
		vec2 texture;
		vec normal;
	};

	BaseTerrain(char* filename);
	~BaseTerrain();
	void Render() {
		FlushBuffers();
	}

protected:	
	void Initialize();
	void LoadHeightMap();
	void NormalizeHeightMap();
	void CalculateNormals();
	void ShutdownHeightMap();
	void CalculateTextureCoordinates();

private:
	HeightMapType* m_heightmap;
	int m_terrainwidth;
	int m_terrainheight;
	char* m_filename;
};



}
