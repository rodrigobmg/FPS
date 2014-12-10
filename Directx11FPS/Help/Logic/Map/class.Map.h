#pragma once
#include "class.Sky.h"
#include "class.BaseTerrain.h"
#include "../../Graphic/Shaders/Shaders.h"

namespace Map 
{

class Base : protected Sky 
{
protected:
	const char* m_name;
	BaseTerrain* m_terrain;
	Shaders::Terrain::Shader* m_tershader;

public:
	Base() {
		m_name = NULL;
		m_terrain = 0;
		m_tershader = 0;
	}
	void Load(const char* name, D3DXMATRIX view) {
		m_name = name;

		Shaders::MatrixList mat;
		mat.world = *gDirect->GetWorldMatrix();
		mat.projection = *gDirect->GetProjectionMatrix();
		mat.view = view;

		m_tershader = new Shaders::Terrain::Shader();
		m_tershader->Initialize();
		m_tershader->Features()->light.ambientColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		m_tershader->Features()->light.diffuseColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		m_tershader->Features()->light.lightDirection = D3DXVECTOR3(-0.5f, -1.0f, 0.0f);
		m_tershader->Features()->light.padding = 0;
		m_tershader->Features()->texture.Add(L"../Media/Textures/dirt01.dds");
		m_tershader->Features()->matrix = mat;
		m_tershader->Features()->matrix.view = view;
		D3DXMatrixScaling(&m_tershader->Features()->matrix.world, 100.0f, 100.0f, 100.0f);

		m_terrain = new BaseTerrain("../Media/Map/heightmap01.bmp");
	}
	const char* getName() {
		return m_name;
	}
	void Render(D3DXMATRIX& view) {
		if(m_name == NULL) return;

		// Projekcna matica teraz a v inite sa lisi...
		m_tershader->Features()->matrix.view = view;
		m_tershader->Render();
		m_terrain->Render();	
	}
	~Base() {
		SAFE_DELETE(m_terrain)
		SAFE_DELETE(m_tershader)
	}
};


}