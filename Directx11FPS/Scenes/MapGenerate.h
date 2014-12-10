#ifndef H_MapGenerate
#define H_MapGenerate
#include "./Global.h"
#include "../base/class.Scene.h"
#include "./classes/Render/Base/class.Draw.h"

class MapGenerate : public Scene
{	
	public :
	MapGenerate();
	~MapGenerate();
	PLUGIN Init();
	PLUGIN Frame(double d);

	int Click(float x, float y);

private:
	void Generate(int point, float force, float radius, float sufix = 10.0f);
	void RenderPoints();
	void RenderTriangles();
	void PrepareTriangles();
 
	Vertex * points;; 
	struct {
		int x;
		int y;
	} size;


	LPDIRECT3DVERTEXBUFFER9 m_pTerrainVB; //vertex buffer
    LPDIRECT3DINDEXBUFFER9  m_pTerrainIB; //index buffer

    DWORD                   m_dwTerrainVertices, //vertex count
                            m_dwTerrainPrimitives; //primitive count


};
#endif