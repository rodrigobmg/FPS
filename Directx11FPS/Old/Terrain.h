//--------------------------------------------------------------------------------------
// Terrain.h
// PIX Workshop GDC2007
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11core.h>
#include <d3dx11async.h>
#include <D3DX10math.h>
#include "../Base/class.File.h"

struct TERRAIN_VERTEX
{
    D3DXVECTOR3 pos;
    D3DXVECTOR3 norm;
    D3DXVECTOR2 uv;
};

struct GRASS_VERTEX
{
    D3DXVECTOR3 pos;
    D3DXVECTOR2 uv;
};

struct BOUNDING_BOX
{
    D3DXVECTOR3 min;
    D3DXVECTOR3 max;
};

struct TERRAIN_TILE
{
    ID3D11Buffer* pVB10;
    UINT NumVertices;
    TERRAIN_VERTEX* pRawVertices;
    D3DXVECTOR4 Color;
    BOUNDING_BOX BBox;
};

class CTerrain
{
private:
	ID3D11Device* m_pDev10dev;
    ID3D11DeviceContext* m_pDev10;
    UINT m_SqrtNumTiles;
    UINT m_NumTiles;
    UINT m_NumSidesPerTile;
    TERRAIN_TILE* m_pTiles;
    float m_fWorldScale;
    float m_fHeightScale;
    UINT m_HeightMapX;
    UINT m_HeightMapY;
    float* m_pHeightBits;

    UINT m_NumIndices;
    ID3D11Buffer* m_pTerrainIB10;
    SHORT* m_pTerrainRawIndices;

    UINT m_NumGrassBlades;
    float m_fGrassWidth;
    float m_fGrassHeight;
    ID3D11Buffer* m_pGrassVB10;
    ID3D11Buffer** m_ppGrassIB10;

    UINT m_NumDirections;
    D3DXVECTOR3* m_pDirections;

public:
                            CTerrain();
                            ~CTerrain();

    void                    OnLostDevice();
    void                    OnDestroyDevice();
    HRESULT                 OnCreateDevice( ID3D11Device* dev, ID3D11DeviceContext* con );

    HRESULT                 LoadTerrain( WCHAR* strHeightMap, UINT SqrtNumTiles, UINT NumSidesPerTile,
                                         float fWorldScale, float fHeightScale,
                                         UINT NumGrassBlades, float fGrassWidth, float fGrassHeight );
    float                   GetHeightForTile( UINT iTile, D3DXVECTOR3* pPos );
    float                   GetHeightOnMap( D3DXVECTOR3* pPos );
    D3DXVECTOR3             GetNormalOnMap( D3DXVECTOR3* pPos );
    void                    RenderTile( TERRAIN_TILE* pTile );
    void                    RenderGrass( D3DXVECTOR3* pViewDir, UINT NumInstances );

    float                   GetWorldScale()
    {
        return m_fWorldScale;
    }
    ID3D11Buffer* GetTerrainIB10()
    {
        return m_pTerrainIB10;
    }
    UINT                    GetNumTiles()
    {
        return m_NumTiles;
    }
    TERRAIN_TILE* GetTile( UINT iTile )
    {
        return &m_pTiles[iTile];
    }

protected:
    D3DXVECTOR2             GetUVForPosition( D3DXVECTOR3* pPos );
    HRESULT                 LoadBMPImage( WCHAR* strHeightMap );
    HRESULT                 GenerateTile( TERRAIN_TILE* pTile, BOUNDING_BOX* pBBox );
    HRESULT                 CreateTileResources( TERRAIN_TILE* pTile );
    HRESULT                 CreateGrass();
};

float RPercent();
