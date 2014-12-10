#include "MapGenerate.h"
#include "./classes/UI/Base/class.Input.h"
#include "./classes/App/class.Direct.h"

MapGenerate::MapGenerate() {
	size.x = 1000;
	size.y = 1000;

	points = new Vertex[size.x * size.y];
	PrepareTriangles();
}
MapGenerate::~MapGenerate() {
	SAFE_DELETE_ARRAY(points);
}
PLUGIN MapGenerate::Init() {
	UINT a, x, z;
	x = z = 0;
	for(a=0; a < size.x*size.y; a++) {
		if(a % size.x == 0) {
			x++;
			z = 0;
		} else {
			z++;
		}

		points[a].color = D3DCOLOR_ARGB(255, 200, 200, 200);
		points[a].x = x * 1.0f;
		points[a].y = 0.0f;
		points[a].z = z * 1.0f;
	}
	return PLUGIN_CONTINUE;
}
PLUGIN MapGenerate::Frame(double d) {
	// Nacitavaj vstup, napriklad...
	if(Input::Press('K')) {
		int point = Click(100, 100);
		Generate(point, 1000.0f, 2000.0f, 1.0f);
	}
	if(Input::Press('L')) {
		float x, y, force, radius;
		x = Math::Random(0.0f, 900.0f);
		y = Math::Random(0.0f, 900.0f);
		force = Math::Random(0.0f, 900.0f);
		radius = Math::Random(0.0f, 900.0f);
		
		int point = Click(x, y);
		Generate(point, force, radius, 1.0f);
	}

	RenderPoints();
	//RenderTriangles();
	return PLUGIN_CONTINUE;
}
int MapGenerate::Click(float x, float y) {
	int ix = floor(x);
	int iy = floor(y);
	int point = ix*iy + iy;
	if(point < 0 || point >= size.x*size.y) {
		assert(false);
		return 0;
	}
	return point;
}
void MapGenerate::Generate(int point, float force, float radius, float sufix)
{
	UINT a;
	vec temp;
	float distance, percent, x;

	for(a=0; a < size.x*size.y; a++) {
		temp = (vec) points[point] - (vec) points[a]; 
		distance = temp.Length();
		if(distance > radius) continue;
		if(!distance) continue;
		// + Vypocitaj silu podla radiusu...
		// Napr. percentualne bude linearna rovna priamka a pod. takze aj tak dame len a len priamku...
		/*
			force....distance 0
			x force... dtistance 100;
		*/
		x = force / distance;
		points[a].y += x;
	}
	points[point].y += force;
}/*
void MapGenerate::RenderPointsAsTriangles() 
{	
	UINT a, x, y;
	x = y = 0;
	Vertex verts[2]; 

	for(a=0; a < size.x*size.y; a++) {
		if(a % size.x == 0) {
			x++;
			y = 0;
		} else {
			y++;
		}

		if(y % size.y != 0) {
			verts[0] = points[x*y + y];
			verts[1] = points[x*y + y + 1];
		}
		G::Direct->zariadenie->DrawPrimitiveUP(D3DPT_LINELIST, 1, &verts, sizeof(Vertex));
	}

}*/
void MapGenerate::RenderPoints() {
	G::Direct->zariadenie->SetFVF(VertexType);
	G::Direct->zariadenie->DrawPrimitiveUP(D3DPT_POINTLIST, size.x*size.y, points, sizeof(Vertex));
}
void MapGenerate::PrepareTriangles() {
	// Premenne
	m_pTerrainVB = NULL;
	m_pTerrainIB = NULL;
	m_dwTerrainVertices = (size.x + 1) * (size.y + 1);
	m_dwTerrainPrimitives = size.x * size.y * 2;

	Vertex* pVertexData; //pointer to vertex
	short* pIndexData; //pointer to index

	// BUffer
	G::Direct->zariadenie->CreateVertexBuffer(sizeof(Vertex)*m_dwTerrainVertices,
                                      D3DUSAGE_WRITEONLY,
                                      VertexType,
                                      D3DPOOL_MANAGED,
                                      &m_pTerrainVB,
                                      NULL);

	m_pTerrainVB->Lock(0,0,(void**)&pVertexData,0);
	unsigned y;
	for(y=0; y < (size.y + 1);++y)
	{
		for(unsigned x = 0;x < (size.x + 1);++x)
		{
			pVertexData[x + y * (size.x + 1)].x = (float)x;
			pVertexData[x + y * (size.x + 1)].y = (float)y;
			pVertexData[x + y * (size.x + 1)].z = 1.0f;
			pVertexData[x + y * (size.x + 1)].color = 0xffffffff; //color
		}
	}
	m_pTerrainVB->Unlock();

	// Index buffer
	G::Direct->zariadenie->CreateIndexBuffer(sizeof(short)*m_dwTerrainPrimitives*3,
                                     D3DUSAGE_WRITEONLY,
                                     D3DFMT_INDEX16,
                                     D3DPOOL_MANAGED,
                                     &m_pTerrainIB,
                                     NULL);

	m_pTerrainIB->Lock(0,0,(void**)&pIndexData,0);

	for(y = 0;y < size.y;++y)
	{
		for(unsigned x = 0;x < size.x;++x)
		{
			*pIndexData++ = x + y * (size.x + 1); //v1
			*pIndexData++ = x + 1 + y * (size.x + 1); //v2
			*pIndexData++ = x + 1 + (y + 1) * (size.x + 1); //v4

			*pIndexData++ = x + y * (size.x + 1); //v1
			*pIndexData++ = x + 1 + (y + 1) * (size.x + 1); //v4
			*pIndexData++ = x + (y + 1) * (size.x + 1); //v3
		}
	}

	m_pTerrainIB->Unlock();

}
void MapGenerate::RenderTriangles() {
	G::Direct->zariadenie->SetStreamSource(0, m_pTerrainVB, 0, sizeof(Vertex));
	G::Direct->zariadenie->SetIndices(m_pTerrainIB);
	G::Direct->zariadenie->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
											0,
											0,
											m_dwTerrainVertices,
											0,
											m_dwTerrainPrimitives);
}