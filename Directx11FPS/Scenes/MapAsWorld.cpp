#include "MapAsWorld.h"
#include "../Help/Graphic/Shaders/Shaders.h"
extern Hud::Manager* gHud;

using namespace Shapes;

class MyCamera : public Camera::Camera {
public:	
	MyCamera() {
		Base::SetPosition(vec(0.0f, 0.0f, -10.0f));
		Base::PrepareMatrix();
	}
};

Tutorial::Tutorial() 
{
	// Priprav kameru
	m_camera = new MyCamera;

	// Priprav debug shader
	m_colorshader = new Shaders::ColorObject::Shader();
	m_colorshader->Initialize();
	m_colorshader->Features()->color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	gIDraw_Defaultshader = m_colorshader;
		
	// Priprav svet
	Map::Base* mapa = new Map::Base();
	mapa->Load("default", m_camera->Base::GetViewMatrix());
	gWorld->setMap(mapa);
	gWorld->setCamera(m_camera);

	// Priprav hud
	gHud->Initialize(
		gGame->getScreenInfo(), 
		Shaders::MatrixList( *gDirect->GetWorldMatrix(), *gDirect->GetOrthoMatrix(), m_camera->Base::GetViewMatrix() ) 
	);
	m_text = gHud->Request("fontdata.txt", "font.dds");
	gHud->Add(m_text);
	m_text->Set("qwertyuiopasdfghjklzxcvbnm!@#$%^&*()", vec2(100.f, 100.f));
	m_text->setColor( D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f) );
}


Tutorial::~Tutorial() {
	SAFE_DELETE(m_camera)
	//SAFE_DELETE(m_LightShader)
	SAFE_DELETE(m_colorshader)

	SAFE_DELETE(m_text)
	SAFE_DELETE(gHud)
}

void Tutorial::Frame(SFrame& s) 
{				
	//  Updatni matice - boli zrejme prepisane pre HUD
	m_colorshader->Features()->matrix.world = *gDirect->GetWorldMatrix();
	m_colorshader->Features()->matrix.projection = *gDirect->GetProjectionMatrix();
	m_colorshader->Features()->matrix.view = m_camera->Base::GetViewMatrix();
	
	// Construct the frustum.
	Frustum* m_Frustum = m_camera->getFrustum();
	m_Frustum->setColor( vec4(0.0, 1.0, 0.0, 0.0) );
	m_Frustum->Shapes::Base::Draw();
}