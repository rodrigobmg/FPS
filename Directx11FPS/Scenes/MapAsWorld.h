#pragma once
#include <exception>
#include "../Help/App/class.Direct.h"
#include "../Help/Logic/Camera/class.Camera.h"
#include "../Help/Shapes/Shapes.h"
#include "../Help/Graphic/Hud/class.Manager.h"
#include "../System/class.World.h"

extern Direct* gDirect;
extern Game* gGame;
extern Hud::Manager* gHud;
extern World* gWorld;

class Tutorial : public Module 
{

public:	
	Camera::Camera* m_camera;
	Shaders::ColorObject::Shader* m_colorshader;
	
	Hud::Text* m_text;
	Hud::FontType* m_font;
	Shaders::Font::Shader* m_fontshader;

	Tutorial();
	~Tutorial();
	void Frame(SFrame& s);
};