#include "class.Terrain.h"
#include "../Math/Math.h"
#include "../Base/QuickDepthSort.h"
#include "../App/class.Direct.h"
#include "../../Global.h"
#include <assert.h>

#define HEIGHT_INDEX( a, b ) ( (b)*m_HeightMapX + (a) )
#define LINEAR_INTERPOLATE(a,b,x) (a*(1.0f-x) + b*x)

static vec s_vDirections[8] =
{
	vec( -1, 0, 0 ),			//left
	vec( -0.707f, 0, 0.707f ),	//up-left
	vec( 0, 0, 1 ),				//up
	vec( 0.707f, 0, 0.707f ),	//up-right
	vec( 1, 0, 0 ),				//right
	vec( 0.707f, 0, -0.707f ),	//down-right
	vec( 0, 0, -1 ),			//down
	vec( -0.707f, 0, -0.707f )  //down-left
};


Terrain::Terrain() {
   m_pTiles = NULL;
   m_HeightMapX = 0;
   m_HeightMapY = 0;
   m_pHeightBits= NULL;
   m_NumIndices = 0;
   m_pTerrainIB11 = NULL;
   m_pTerrainRawIndices = NULL;
   m_pGrassVB11 = NULL;
   m_ppGrassIB11 = NULL;
   m_NumDirections = 0;
   m_pDirections =NULL;
}

void Terrain::Init(		
	wchar_t* strHeightMap, 
	UINT SqrtNumTiles, 
	UINT NumSidesPerTile, 
	float fWorldScale, 
	float fHeightScale,
	UINT NumGrassBlades, 
	float fGrassWidth, 
	float fGrassHeight 
) {
	// Store variables
	m_SqrtNumTiles = SqrtNumTiles;
	m_fWorldScale = fWorldScale;
	m_fHeightScale = fHeightScale;
	m_NumSidesPerTile = NumSidesPerTile;
	m_NumTiles = SqrtNumTiles * SqrtNumTiles;
	m_NumGrassBlades = NumGrassBlades;
	m_fGrassWidth = fGrassWidth;
	m_fGrassHeight = fGrassHeight;
	m_strHeightMap = strHeightMap;
}

Terrain::~Terrain() {
	for( UINT i = 0; i < m_NumTiles; i++ ) {
		SAFE_DELETE_ARRAY( m_pTiles[i].pRawVertices );
	}
	SAFE_DELETE_ARRAY( m_pTiles );
	SAFE_DELETE_ARRAY( m_pHeightBits );
	SAFE_DELETE_ARRAY( m_pTerrainRawIndices );
}
void Terrain::OnDestroyDevice()
{
	for( UINT i = 0; i < m_NumTiles; i++ ) {
		SAFE_RELEASE( m_pTiles[i].pVB11 );
	}
	SAFE_RELEASE( m_pTerrainIB11 );
	SAFE_RELEASE( m_pGrassVB11 );
	for( UINT i = 0; i < m_NumDirections; i++ ) 
		SAFE_RELEASE( m_ppGrassIB11[i] );

	SAFE_DELETE_ARRAY( m_ppGrassIB11 );
	SAFE_DELETE_ARRAY( m_pDirections );
}

void Terrain::OnCreateDevice()
{
	if( 0 == m_NumTiles ) assert(0);

	// Create the terrain tile vertex buffers
	for( UINT i = 0; i < m_NumTiles; i++ ){
		CreateTileResources( &m_pTiles[i] );
	}

	// Create the index buffer
	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.ByteWidth = m_NumIndices * sizeof( short );
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_pTerrainRawIndices;

	m_direct->GetDevice()->CreateBuffer( &BufferDesc, &InitData, &m_pTerrainIB11 );
	CreateGrass();
}

void Terrain::LoadTerrain()
{
	// Load the image
	LoadBMPImage();

	// Create tiles
	m_pTiles = new TERRAIN_TILE[ m_NumTiles ];

	UINT iTile = 0;
	float zStart = -m_fWorldScale / 2.0f;
	float zDelta = m_fWorldScale / ( float )m_SqrtNumTiles;
	float xDelta = m_fWorldScale / ( float )m_SqrtNumTiles;
	for( UINT z = 0; z < m_SqrtNumTiles; z++ )
	{
		float xStart = -m_fWorldScale / 2.0f;
		for( UINT x = 0; x < m_SqrtNumTiles; x++ )
		{
			AABBMM BBox;
			BBox.setSizeMM( vec( xStart, 0.0f, zStart ), vec( xStart + xDelta, 0.0f, zStart + zDelta ) );
			GenerateTile( &m_pTiles[iTile], &BBox );

			iTile++;
			xStart += xDelta;
		}
		zStart += zDelta;
	}

	// Create the indices for the tile strips
	m_NumIndices = ( m_NumSidesPerTile + 2 ) * 2 * ( m_NumSidesPerTile )- 2;
	m_pTerrainRawIndices = new short[ m_NumIndices ];

	short vIndex = 0;
	UINT iIndex = 0;
	for( UINT z = 0; z < m_NumSidesPerTile; z++ ) 
	{
		for( UINT x = 0; x < m_NumSidesPerTile + 1; x++ ) {
			m_pTerrainRawIndices[iIndex] = vIndex;
			iIndex++;
			m_pTerrainRawIndices[iIndex] = vIndex + ( short )m_NumSidesPerTile + 1;
			iIndex++;
			vIndex++;
		}
		
		if( z != m_NumSidesPerTile - 1 ) {
			// add a degenerate tri
			m_pTerrainRawIndices[iIndex] = vIndex + ( short )m_NumSidesPerTile;
			iIndex++;
			m_pTerrainRawIndices[iIndex] = vIndex;
			iIndex++;
		}
	}
}
float Terrain::GetHeightForTile( UINT iTile, vec* pPos ) {
	// TODO impl
	return 0.0f;
}
float Terrain::GetHeightOnMap( vec* pPos ) {
	// move x and z into [0..1] range
	vec2 uv = GetUVForPosition( pPos );
	float x = uv.x;
	float z = uv.y;

	// scale into heightmap space
	x *= m_HeightMapX;
	z *= m_HeightMapY;
	x += 0.5f;
	z += 0.5f;
	if( x >= m_HeightMapX - 1 )
	x = ( float )m_HeightMapX - 2;
	if( z >= m_HeightMapY - 1 )
	z = ( float )m_HeightMapY - 2;
	z = max( 0, z );
	x = max( 0, x );

	// bilinearly interpolate
	unsigned long integer_X = unsigned long( x );
	float fractional_X = x - integer_X;

	unsigned long integer_Z = unsigned long( z );
	float fractional_Z = z - integer_Z;

	float v1 = m_pHeightBits[ HEIGHT_INDEX( integer_X,integer_Z ) ];
	float v2 = m_pHeightBits[ HEIGHT_INDEX( integer_X + 1,integer_Z ) ];
	float v3 = m_pHeightBits[ HEIGHT_INDEX( integer_X,integer_Z + 1 ) ];
	float v4 = m_pHeightBits[ HEIGHT_INDEX( integer_X + 1,integer_Z + 1 ) ];

	float i1 = LINEAR_INTERPOLATE( v1 , v2 , fractional_X );
	float i2 = LINEAR_INTERPOLATE( v3 , v4 , fractional_X );
	float result = LINEAR_INTERPOLATE( i1 , i2 , fractional_Z );
	return result;
}
vec Terrain::GetNormalOnMap( vec* pPos )
{
	// Calculate the normal
	float xDelta = ( m_fWorldScale / ( float )m_SqrtNumTiles ) / ( float )m_NumSidesPerTile;
	float zDelta = ( m_fWorldScale / ( float )m_SqrtNumTiles ) / ( float )m_NumSidesPerTile;

	vec vLeft = *pPos - vec( xDelta, 0, 0 );
	vec vRight = *pPos + vec( xDelta, 0, 0 );
	vec vUp = *pPos + vec( 0, 0, zDelta );
	vec vDown = *pPos - vec( 0, 0, zDelta );

	vLeft.y = GetHeightOnMap( &vLeft );
	vRight.y = GetHeightOnMap( &vRight );
	vUp.y = GetHeightOnMap( &vUp );
	vDown.y = GetHeightOnMap( &vDown );

	vec e0 = vRight - vLeft;
	vec e1 = vUp - vDown;
	vec norm;
	norm = Math::CROSS(e1, e0);
	norm.Normalize();
	return norm;
}
void Terrain::RenderGrass( vec* pViewDir, UINT NumInstances )
{
	// Pick the IB
	float fMaxDot = -1.0f;
	UINT maxDir = 0;
	for( UINT i = 0; i < m_NumDirections; i++ )
	{
		float fDot = Math::DOT3(*pViewDir, m_pDirections[i] );
		if( fDot > fMaxDot ) {
			fMaxDot = fDot;
			maxDir = i;
		}
	}

    m_direct->GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    m_direct->GetDeviceContext()->IASetIndexBuffer( m_ppGrassIB11[maxDir], DXGI_FORMAT_R16_UINT, 0 );
    UINT stride = sizeof( GRASS_VERTEX );
    UINT offset = 0;
    m_direct->GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_pGrassVB11, &stride, &offset );
    m_direct->GetDeviceContext()->DrawIndexedInstanced( m_NumGrassBlades * 6, NumInstances, 0, 0, 0 );
}

void Terrain::RenderTile(TERRAIN_TILE* pTile) {
	UINT stride = sizeof( TERRAIN_VERTEX );
    UINT offset = 0;
    m_direct->GetDeviceContext()->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
    m_direct->GetDeviceContext()->IASetVertexBuffers( 0, 1, &pTile->pVB11, &stride, &offset );
    m_direct->GetDeviceContext()->DrawIndexed( m_NumIndices, 0, 0 );
}

vec2 Terrain::GetUVForPosition( vec* pPos ) {
	vec2 uv;
	uv.x = ( pPos->x / m_fWorldScale ) + 0.5f;
	uv.y = ( pPos->z / m_fWorldScale ) + 0.5f;
	return uv;
}

void Terrain::LoadBMPImage()
{
	FILE* fp = NULL;
	_wfopen_s( &fp, m_strHeightMap, L"rb" );
	if(!fp) {
		throw new FileNotFound(m_strHeightMap);
		return;
	}

	// read the bfh
	BITMAPFILEHEADER bfh;
	fread( &bfh, sizeof( BITMAPFILEHEADER ), 1, fp );
	unsigned long toBitmapData = bfh.bfOffBits;
	unsigned long bitmapSize = bfh.bfSize;

	// read the header
	BITMAPINFOHEADER bih;
	fread( &bih, sizeof( BITMAPINFOHEADER ), 1, fp );
	if( bih.biCompression != BI_RGB ) { fclose( fp ); throw new Terrain::FileProcessing(); return; }

	// alloc memory
	unsigned long U = m_HeightMapX = bih.biWidth;
	unsigned long V = m_HeightMapY = abs( bih.biHeight );
	m_pHeightBits = new float[ U * V ];
	//if( !m_pHeightBits ) return E_OUTOFMEMORY;

	// find the step size
	unsigned long iStep = 4;
	if( 24 == bih.biBitCount ) iStep = 3;

	// final check for size
	unsigned long UNew = ( ( U * iStep * 8 + 31 ) & ~31 ) / ( 8 * iStep );
	if( bitmapSize < UNew * V * iStep * sizeof( BYTE ) ) { 
		fclose( fp ); 
		throw new Terrain::FileProcessing(); 
		return;
	}

	// seek
	fseek( fp, toBitmapData, SEEK_SET );

	// read in the bits
	BYTE* pBits = new BYTE[ bitmapSize ];
	fread( pBits, bitmapSize, 1, fp );

	// close the file
	fclose( fp );

	// Load the Height Information
	unsigned long iHeight = 0;
	unsigned long iBitmap = 0;
	for( unsigned long y = 0; y < V; y++ )
	{
		iBitmap = y * UNew * iStep;
		for( unsigned long x = 0; x < U * iStep; x += iStep )
		{
			m_pHeightBits[iHeight] = 0;
			for( unsigned long c = 0; c < iStep; c++ ) {
				m_pHeightBits[iHeight] += pBits[ iBitmap + c ];
			}
			m_pHeightBits[iHeight] /= ( FLOAT )( iStep * 255.0 );
			m_pHeightBits[iHeight] *= m_fHeightScale;

			iHeight ++;
			iBitmap += iStep;
		}
	}
	SAFE_DELETE_ARRAY( pBits );
}



void Terrain::GenerateTile( TERRAIN_TILE* pTile, AABBMM* pBBox )
{
	// Alloc memory for the vertices
	pTile->NumVertices = ( m_NumSidesPerTile + 1 ) * ( m_NumSidesPerTile + 1 );
	pTile->pRawVertices = new TERRAIN_VERTEX[ pTile->NumVertices ];

	pTile->BBox = *pBBox;
	pTile->Color.x = 0.60f + Math::RPercent() * 0.40f;
	pTile->Color.y = 0.60f + Math::RPercent() * 0.40f;
	pTile->Color.z = 0.60f + Math::RPercent() * 0.40f;
	pTile->Color.w = 1.0f;
	
	vec min = pBBox->getAbsMin();
	vec max = pBBox->getAbsMax();
	UINT iVertex = 0;
	float zStart = min.z;
	float xDelta = ( max.x - min.x ) / ( float )m_NumSidesPerTile;
	float zDelta = ( max.z - min.z ) / ( float )m_NumSidesPerTile;

	// Loop through terrain vertices and get height from the heightmap
	for( UINT z = 0; z < m_NumSidesPerTile + 1; z++ )
	{
		float xStart = min.x;
		for( UINT x = 0; x < m_NumSidesPerTile + 1; x++ )
		{
			vec pos( xStart,0,zStart );
			vec norm;
			pos.y = GetHeightOnMap( &pos );
			pTile->pRawVertices[iVertex].pos = pos;
			pTile->pRawVertices[iVertex].uv = GetUVForPosition( &pos );
			pTile->pRawVertices[iVertex].uv.y = 1.0f - pTile->pRawVertices[iVertex].uv.y;
			pTile->pRawVertices[iVertex].norm = GetNormalOnMap( &pos );

			iVertex ++;
			xStart += xDelta;
		}
		zStart += zDelta;
	}
}

void Terrain::CreateTileResources( TERRAIN_TILE* pTile )
{
	HRESULT hr;
	D3D11_BUFFER_DESC BufferDesc;
    BufferDesc.ByteWidth = m_pTiles->NumVertices * sizeof( TERRAIN_VERTEX );
    BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    BufferDesc.CPUAccessFlags = 0;
    BufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = pTile->pRawVertices;
    V( m_direct->GetDevice()->CreateBuffer( &BufferDesc, &InitData, &pTile->pVB11 ) );
}
HRESULT Terrain::CreateGrass()
{
	HRESULT hr = S_OK;
	float fTileSize = m_fWorldScale / ( float )m_SqrtNumTiles;
	fTileSize /= 2.0f;
	float fGrassWidth = m_fGrassWidth / 2.0f;
	float fGrassHeight = m_fGrassHeight;
	vec* pGrassCenters = new vec[ m_NumGrassBlades ];

	for( UINT i = 0; i < m_NumGrassBlades; i++ ) {
		pGrassCenters[i].x = Math::RPercent() * fTileSize;
		pGrassCenters[i].y = 0.0f;
		pGrassCenters[i].z = Math::RPercent() * fTileSize;
	}

	GRASS_VERTEX* pVertices = NULL;
    D3D11_BUFFER_DESC BufferDesc;

    BufferDesc.ByteWidth = m_NumGrassBlades * 4 * sizeof( GRASS_VERTEX );
    BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    BufferDesc.CPUAccessFlags = 0;
    BufferDesc.MiscFlags = 0;
    pVertices = new GRASS_VERTEX[ m_NumGrassBlades * 4 ];

	UINT vIndex = 0;
	for( UINT i = 0; i < m_NumGrassBlades; i++ )
	{
		vec vRandRight( Math::RPercent(), 0, Math::RPercent() );
		vRandRight.Normalize();

		pVertices[vIndex  ].pos = pGrassCenters[i] - vRandRight * fGrassWidth;
		pVertices[vIndex  ].uv = vec2( 0.0f, 1.0f );
		pVertices[vIndex + 1].pos = pGrassCenters[i] + vRandRight * fGrassWidth;
		pVertices[vIndex + 1].uv = vec2( 1.0f, 1.0f );
		pVertices[vIndex + 2].pos = pVertices[vIndex + 1].pos + vec( 0.0f, fGrassHeight, 0.0f );
		pVertices[vIndex + 2].uv = vec2( 1.0f, 0.0f );
		pVertices[vIndex + 3].pos = pVertices[vIndex  ].pos + vec( 0.0f, fGrassHeight, 0.0f );
		pVertices[vIndex + 3].uv = vec2( 0.0f, 0.0f );
		vIndex += 4;
	}
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	V_RETURN( m_direct->GetDevice()->CreateBuffer( &BufferDesc, &InitData, &m_pGrassVB11 ) );
	SAFE_DELETE_ARRAY( pVertices );

	// Alloc indices and distances
	short* pGrassIndices = new short[ m_NumGrassBlades ];
	float* pGrassDistances = new float[ m_NumGrassBlades ];
	m_NumDirections = 16;
	m_ppGrassIB11 = new ID3D11Buffer*[ m_NumDirections ];
	m_pDirections = new vec[ m_NumDirections ];

	vec vStartDir( -1,0,0 );
	float fAngleDelta = ( D3DX_PI * 2.0f ) / ( float )( m_NumDirections );
	for( UINT i = 0; i < m_NumDirections; i++ )
	{
		D3DXMATRIX mRot;
		D3DXMatrixRotationY( &mRot, i * fAngleDelta );
		D3DXVec3TransformNormal( &(D3DXVECTOR3) m_pDirections[i], &(D3DXVECTOR3) vStartDir, &mRot );

		// init indices and distances
		for( UINT g = 0; g < m_NumGrassBlades; g++ ) {
			pGrassIndices[g] = ( short )g;
			pGrassDistances[g] = - Math::DOT3( m_pDirections[i], pGrassCenters[g] );
		}

		// sort indices
		QuickDepthSort( pGrassIndices, pGrassDistances, 0, m_NumGrassBlades - 1 );
		short* pIndices = NULL;

		BufferDesc.ByteWidth = m_NumGrassBlades * 6 * sizeof( short );
		BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		BufferDesc.CPUAccessFlags = 0;
		BufferDesc.MiscFlags = 0;
		pIndices = new short[ m_NumGrassBlades * 6 ];

		UINT iIndex = 0;
		for( UINT g = 0; g < m_NumGrassBlades; g++ )
		{
			//Starting vert
			short GrassIndex = pGrassIndices[g] * 4;
			//Tri1
			pIndices[iIndex  ] = GrassIndex;
			pIndices[iIndex + 1] = GrassIndex + 3;
			pIndices[iIndex + 2] = GrassIndex + 1;
			//Tri2
			pIndices[iIndex + 3] = GrassIndex + 1;
			pIndices[iIndex + 4] = GrassIndex + 3;
			pIndices[iIndex + 5] = GrassIndex + 2;
			iIndex += 6;
		}

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pIndices;
		V_RETURN( m_direct->GetDevice()->CreateBuffer( &BufferDesc, &InitData, &m_ppGrassIB11[i] ) );
		SAFE_DELETE_ARRAY( pIndices );
	}									

	SAFE_DELETE_ARRAY( pGrassIndices );
	SAFE_DELETE_ARRAY( pGrassDistances );
	SAFE_DELETE_ARRAY( pGrassCenters );
	return hr;
}