#pragma once
#include "DirectFPS.h"
#include "../Help/App/class.Direct.h"
#include <exception>

#include "../Help/Graphic/class.Camera.h"
#include "../Help/Graphic/Base/class.Model.h"
#include "../Help/Graphic/Shader/Shaders.h"
#include "../Help/Graphic/Base/class.Light.h"
#include "../Help/Graphic/Base/class.Bitmap.h"
#include "../Help/Graphic/Base/class.Text.h"
#include "../Help/Sound/class.WAW3D.h"

extern Direct* gDirect;
extern Game* gGame;

class Tutorial : public Module {
public:
	Camera* m_Camera;
	Bitmap* m_Bitmap;
	TextureShader* m_TextureShader;
	Text* m_Text;
	WAW3D* sound;

	Tutorial() {
		bool result;
		m_Camera = new Camera;
		
		// Create the camera object.
		m_Camera = new Camera;
		if(!m_Camera) {
			throw new std::exception("FALSE");
		}

		// Create the texture shader object.
		m_TextureShader = new TextureShader;
		if(!m_TextureShader) {
			throw new std::exception("FALSE");
		}

		// Initialize the texture shader object.
		m_TextureShader->Init(gDirect->GetDevice(), L"../Media/texture");

		// Create the bitmap object.
		m_Bitmap = new Bitmap;

		// Initialize the bitmap object.
		m_Bitmap->Initialize(gDirect->GetDevice(), 800, 600, L"../Media/seafloor.dds", 256, 256);


		// Create the text object.
		m_Text = new Text;

		D3DXMATRIX baseViewMatrix;
		m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
		m_Camera->Render();
		m_Camera->GetViewMatrix(baseViewMatrix);

		// Initialize the text object.
		result = m_Text->Initialize(gDirect->GetDevice(), gDirect->GetDeviceContext(), NULL, 800, 600, baseViewMatrix);
		if(!result) {
			MessageBox(NULL, L"Could not initialize the text object.", L"Error", MB_OK);
		}

		sound = new WAW3D;
		sound->Init("../Media/sound02.wav");
		D3DVECTOR initpos = {0.f, 0.f, -10.f };
		sound->SetPosition(&initpos);
	}
	~Tutorial() {
		SAFE_DELETE(m_Camera)
		SAFE_DELETE(m_Bitmap)
		SAFE_DELETE(m_TextureShader)
		SAFE_DELETE(sound)

		if(m_Text) {
			m_Text->Shutdown();
			delete m_Text;
			m_Text = 0;
		}

	}

	virtual void Frame() {
		sound->Play();
		m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
		
		D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
		D3DXMATRIX orthoMatrix;

		// Generate the view matrix based on the camera's position.
		m_Camera->Render();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		worldMatrix = *(gDirect->GetWorldMatrix());
		projectionMatrix = *(gDirect->GetProjectionMatrix());
		orthoMatrix = *(gDirect->GetOrthoMatrix());

		// Turn off the Z buffer to begin all 2D rendering.
		gDirect->TurnZBuffer(false);

		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Bitmap->Render(gDirect->GetDeviceContext(), 100, 100);

		// Render the model with the texture shader.
		m_TextureShader->Render(gDirect->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, 
						 m_Bitmap->GetTexture());

		// Turn on the alpha blending before rendering the text.
		gDirect->TurnAlphaBlending(true);
		// Render the text strings.
		m_Text->Render(gDirect->GetDeviceContext(), worldMatrix, orthoMatrix);

		// Turn off alpha blending after rendering the text.
		gDirect->TurnAlphaBlending(false);


		// Turn the Z buffer back on now that all 2D rendering has completed.
		gDirect->TurnZBuffer(true);
	}
};