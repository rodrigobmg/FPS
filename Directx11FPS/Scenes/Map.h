#pragma once
#include "DirectFPS.h"
#include "../Help/App/class.Direct.h"
#include <exception>

#include "../Help/Logic/Camera/class.Camera.h"
#include "../Help/Graphic/Structures/class.RastertekFile.h"
#include "../Help/Graphic/Base/class.Texture.h"
#include "../Help/Graphic/Shaders/Shaders.h"
#include "class.Modellist.h"
#include "../Help/Shapes/Shapes.h"
#include "../Help/Graphic/Hud/class.Driver.h"
#include "../Help/Graphic/Hud/Base/class.Text.h"
#include "../Help/Graphic/Hud/Base/class.FontType.h"
#include "../Help/Logic/Map/class.BaseTerrain.h"

using namespace Shapes;
using namespace Camera;

extern Direct* gDirect;
extern Game* gGame;
extern Hud::Driver* gHudDriver;
extern World* gWorld;
Shaders::ColorObject::Shader* gColorShader;

class Tutorial : public Module 
{
	class CustomCamera : public Camera {
	public:	
		CustomCamera() {
			Base::SetPosition(vec(0.0f, 0.0f, -10.0f));
			Base::PrepareMatrix();
		}
		virtual void Frame() {
			Camera::Frame(gGame->GetDeltaTime());
		}
	};

public:	
	CustomCamera* m_camera;
	
	Shaders::Light::Shader* m_LightShader;
	Structures::RastertekFile* m_Model;
	ModelList* m_ModelList;
	Shaders::ColorObject::Shader* m_ColorShader;
	
	Hud::Text* m_text;
	Hud::FontType* m_font;
	Shaders::Font::Shader* m_fontshader;

	Tutorial() {
		bool result;
		Shaders::MatrixList mat;
		m_camera = new CustomCamera;
		
		Map::Base* map = new Map::Base();
		map->Load("default", m_camera->Base::GetViewMatrix());
		gWorld->setMap( new Map::Base() );
		gWorld->setCamera(m_camera);

		mat.world = *gDirect->GetWorldMatrix();
		mat.projection = *gDirect->GetProjectionMatrix();
		mat.view = m_camera->Base::GetViewMatrix();

		// Create the model list object.
		m_ModelList = new ModelList;
		result = m_ModelList->Initialize(50);
		if(!result) {
			MessageBox(NULL, L"Could not initialize the model list object.", L"Error", MB_OK);
		}
		// Create the model object.
		m_Model =  new Structures::RastertekFile("../Media/Structures/sphere.txt");
	
		// Create the light shader object.
		m_LightShader = new Shaders::Light::Shader();
		m_LightShader->Initialize();
		m_LightShader->Features()->texture.Add(L"../Media/Textures/seafloor.dds");
		m_LightShader->Features()->light.diffuseColor = vec4(1.0, 0.0, 0.0, 1.0);
		m_LightShader->Features()->light.lightDirection = vec(0.0f, 0.0f, 1.0f);

		m_ColorShader = new Shaders::ColorObject::Shader();
		m_ColorShader->Initialize();
		m_ColorShader->Features()->color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		m_ColorShader->Features()->matrix = mat;
		gColorShader = m_ColorShader;

		m_fontshader = new Shaders::Font::Shader();
		m_fontshader->Initialize();
		m_fontshader->Features()->texture.Add(L"../Media/Textures/font.dds");
		m_fontshader->Features()->color = vec4(1.0, 0.0, 0.0, 1.0);
		m_fontshader->Features()->matrix = mat;
		m_fontshader->Features()->matrix.projection = *gDirect->GetOrthoMatrix();

		m_font = new Hud::FontType("../Media/Structures/fontdata.txt");
		m_text = new Hud::Text(m_font, m_fontshader);
		gHudDriver->Add(m_text);
		m_text->Set("qwertyuiopasdfghjklzxcvbnm!@#$%^&*()", vec2(100.f, 100.f));
		m_text->setColor( D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f) );		
	}
	~Tutorial() {
		m_ModelList->Shutdown();

		SAFE_DELETE(m_camera)
		SAFE_DELETE(m_LightShader)
		SAFE_DELETE(m_ColorShader)
		SAFE_DELETE(m_ModelList)

		SAFE_DELETE(m_fontshader)
		SAFE_DELETE(m_font)
		SAFE_DELETE(m_text)
	}

	virtual void Frame(SFrame& s) 
	{		
		Shaders::MatrixList mat;
		D3DXMATRIX *orthoMatrix;
		int modelCount, renderCount, index;
		Sphere *sphere;
		D3DXVECTOR4 color;
		bool renderModel;

		// Get the view, projection, and world matrices from the camera and D3D objects.
		mat.view = m_camera->Base::GetViewMatrix();
		mat.projection = *gDirect->GetProjectionMatrix();
		mat.world = *gDirect->GetWorldMatrix();
		orthoMatrix = gDirect->GetOrthoMatrix();
		
		// Get the number of models that will be rendered.
		modelCount = m_ModelList->GetModelCount();
		
		// Construct the frustum.
		m_ColorShader->Features()->matrix.world = mat.world;
		m_ColorShader->Features()->matrix.projection = mat.projection;
		m_ColorShader->Features()->matrix.view = mat.view;
		Frustum* m_Frustum = m_camera->getFrustum();
		m_Frustum->setColor( vec4(0.0, 1.0, 0.0, 0.0) );
		m_Frustum->Shapes::Base::Draw();

		
		AABBMM a;
		a.setSize(vec(0.f), 1000.f);
		a.setColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
		a.Shapes::Base::Draw();

		AABBMM b[3];
		b[0].setSize(vec(-200.f, 0.f, 0.f), 100.f);
		b[0].setColor(vec4(0.0f, 1.0f, 0.0f, 1.0f));
		b[0].Shapes::Base::Draw();

		b[1].setSize(vec(0.f, 0.f, 0.f), 100.f);
		b[1].setColor(vec4(0.0f, 1.0f, 0.0f, 1.0f));
		b[1].Shapes::Base::Draw();

		b[2].setSize(vec(200.f, 0.f, 0.f), 100.f);
		b[2].setColor(vec4(0.0f, 1.0f, 0.0f, 1.0f));
		b[2].Shapes::Base::Draw();

		AABBMM c;
		c.setSize(vec(0.f), 300.f);
		c.setColor(vec4(0.0f, 0.0f, 1.0f, 1.0f));
		c.Shapes::Base::Draw();
		
		// Initialize the count of models that have been rendered.
		renderCount = 0;
		
		for(index=0; index< modelCount; index++)
		{
			// Get the position and color of the sphere model at this index.
			sphere = m_ModelList->GetData(index);

			// Check if the sphere model is in the view frustum.
			renderModel = true; //m_Frustum.Test(sphere);

			// If it can be seen then render it, if not skip this model and check the next sphere.
			if(renderModel)
			{
				// Move the model to the location it should be rendered at.
				mat.world = *gDirect->GetWorldMatrix();
				D3DXMATRIX a, b;
				D3DXMatrixTranslation(&a, sphere->getOrigin().x, sphere->getOrigin().y, sphere->getOrigin().z); // !!!
				float r = sphere->getRadius();
				D3DXMatrixScaling(&b, r, r, r);
				// ze vraj tieto funkcie robia kopie a a to sa riesi nasobenim
				D3DXMatrixMultiply(&mat.world, &a, &b);
				
				// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
				m_ColorShader->Features()->matrix = mat;
				m_ColorShader->Features()->color = sphere->getColor();
				m_LightShader->Features()->matrix = mat;
				m_LightShader->Features()->light.diffuseColor = sphere->getColor();

				// Render the model using the light shader.
				m_LightShader->Render();
				//m_ColorShader->Render();
				m_Model->Render();

				// Since this model was rendered then increase the count for this frame.
				renderCount++;
			}
		}
	}
};