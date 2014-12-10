#pragma once
#include "DirectFPS.h"
#include "../Help/App/class.Direct.h"
#include <exception>
#include <assert.h>

#include "../Help/Logic/Camera/inter.IMoveable.h"
#include "../Help/Graphic/Shaders/Shaders.h"
#include "../Help/Graphic/Font/class.Text.h"
#include "../Help/Shapes/Shapes.h"

#include "../Help/Graphic/Structures/inter.RenderMode.h"
#include "../Help/Graphic/Structures/inter.IStatic.h"

using namespace Shapes;
using namespace Camera;

extern Direct* gDirect;
extern Game* gGame;

class Tutorial : public Module 
{
	class CustomCamera : public Camera {
	public:	
		CustomCamera() {
			Base::SetPosition(vec(0.0f, 0.0f, -10.0f));
			Base::PrepareMatrix();
		}
		virtual void Frame() {
			Camera::DeltaFrame(gGame->GetDeltaTime());
		}
	};

	struct Lines_VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};


	class DynamicLines : 
		public Structures::ILines,
		public Structures::IStatic<Lines_VertexType>
	{
	public:
		DynamicLines() {
			CreateVStructure(3);
			CreateIStructure(3);
		}
		virtual bool UpdateContext() {
			/*
			vec4 color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
			m_vertices[0].position = D3DXVECTOR3(0.f, 0.f, 0.f);
			m_vertices[0].color = color;
			m_indices[0] = 0;

			m_vertices[1].position = D3DXVECTOR3(100.f, 0.f, 100.f);
			m_vertices[1].color = color;
			m_indices[1] = 1;

			m_vertices[2].position = D3DXVECTOR3(100.f, 0.f, 150.f);
			m_vertices[2].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
			m_indices[2] = 2; // skus 0, 1 a pekne zoraduje
			*/
			return true;
		}
	};

public:	
	CustomCamera* m_Camera;
	DynamicLines* m_Lines;
	Shaders::Color::Shader* m_ColorShader;

	Tutorial() {
		// Create the camera object.
		D3DXMATRIX baseViewMatrix;
		m_Camera = new CustomCamera;
		m_Camera->Base::SetPosition(vec(0.0f, 0.0f, -1.0f));
		baseViewMatrix = m_Camera->Base::GetViewMatrix();
		m_Camera->Base::SetPosition(vec(50.0f, 2.0f, -25.0f));

		// Create the terrain object.
		m_Lines = new DynamicLines;
		m_Lines->Initialize();

		// Create the color shader object.
		m_ColorShader = new Shaders::Color::Shader;

		// Initialize the color shader object.
		m_ColorShader->Initialize();

	}
	~Tutorial() {
		// Release the color shader object.
		if(m_ColorShader) {
			delete m_ColorShader;
			m_ColorShader = 0;
		}
		// Release the camera object.
		if(m_Camera)
		{
			delete m_Camera;
			m_Camera = 0;
		}
		delete m_Lines;
	}

	virtual void Frame() {
		m_Camera->Frame();

		// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
		Shaders::MatrixList mat;
		mat.world = *gDirect->GetWorldMatrix();
		mat.view = m_Camera->Base::GetViewMatrix();
		mat.projection = *gDirect->GetProjectionMatrix();

		// Render the model using the color shader.
		m_ColorShader->Features()->matrix = mat;
		m_ColorShader->Render();
		
		// Render the terrain buffers.
		m_Lines->Render();
	}
};
