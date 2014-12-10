#pragma once
#include "TerrainData.h"
#include "../Graphic/inter.IGraphic.h"
#include "../Base/class.File.h"


/*
	There are no special ways to do this, because it is scene dependent, 
	meaning the method will be different for big open worlds, or closed space worlds. 
	You should research spacial partitioning, which is the topic of your question. 
	You can have separate buffers for your vegetation, then split the viewable space into decent size chunks, 
	then do a simple check to see if the chunk is partially visible, if so, draw it. 
	You are always going to have over draw. The smaller the chunks, the better the culling, however, 
	this will increase the amount of draw calls and cpu work. 
	This is where it will be scene dependent. 
*/

class Terrain : public IGraphicDevice, public File
{
	public:
	Terrain();
	~Terrain();

	void Init(
		wchar_t* strHeightMap, 
		UINT SqrtNumTiles, 
		UINT NumSidesPerTile, 
		float fWorldScale, 
		float fHeightScale,
		UINT NumGrassBlades, 
		float fGrassWidth, 
		float fGrassHeight 
	);
	void LoadTerrain();

	// int CountTriangles(float, float, float);
	float GetHeightForTile( UINT iTile, vec* pPos );
	float GetHeightOnMap( vec* pPos );
	vec  GetNormalOnMap( vec* pPos );	
	float GetWorldScale() { return m_fWorldScale; }
	ID3D11Buffer* GetTerrainIB() { return m_pTerrainIB11; }
	UINT GetNumTiles(){ return m_NumTiles; }
	TERRAIN_TILE* GetTile( UINT iTile ) { return &m_pTiles[iTile];}

	void RenderGrass(vec* pViewDir, UINT NumInstances);
	void RenderTile(TERRAIN_TILE* pTile);

protected:
	vec2 GetUVForPosition(vec* pPos);

	void OnDestroyDevice();
	void OnCreateDevice();

	DynArray <UINT> g_VisibleTileArray;
	DynArray <float> g_VisibleTileArrayDepth;

public:
	UINT m_SqrtNumTiles;
	UINT m_NumTiles;
	UINT m_NumSidesPerTile;
	TERRAIN_TILE* m_pTiles;
	UINT m_NumGrassTiles;

	float m_fWorldScale;
	float m_fHeightScale;
	UINT m_HeightMapX;
	UINT m_HeightMapY;
	float* m_pHeightBits;
	UINT m_NumIndices;
	ID3D11Buffer* m_pTerrainIB11;
	short* m_pTerrainRawIndices;

	UINT m_NumGrassBlades;
	float m_fGrassWidth;
	float m_fGrassHeight;
	ID3D11Buffer* m_pGrassVB11;
    ID3D11Buffer** m_ppGrassIB11;
	UINT m_NumDirections;
	vec* m_pDirections;

	wchar_t* m_strHeightMap;
	void LoadBMPImage();
	void GenerateTile(TERRAIN_TILE* pTile, AABBMM* pBBox);
	void CreateTileResources( TERRAIN_TILE* pTile );
	HRESULT CreateGrass();
};