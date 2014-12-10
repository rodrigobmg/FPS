#include "../Help/App/class.Direct.h"
#include <exception>
#include "../Help/Logic/Camera/class.Camera.h"
#include "../Help/Graphic/Shaders/Shaders.h"

// Trieda pouzita pre oznacenie globalnehostavu v classe
class IGlobalState {
public:
	virtual void GlobalStart() = 0;
	virtual void GlobalEnd() = 0;

	IGlobalState() {
		
	}
};


Shaders::ColorObject::Shader* g_IDraw_Defaultshader = NULL;
// Ale niekde to musim nacitat a vymazat...

class IDraw : public IGlobalState
{
private:
	Shaders::ColorObject::Shader* m_defaultshader;

public:
	static Shaders::ColorObject::Shader* gDefaultshader;
	IDraw(Shaders::ColorObject::Shader* shader = g_IDraw_Defaultshader) {
		m_defaultshader = shader;
	}
};

class IDraww : public IGlobalState
{
private:
	static Shaders::ColorObject::Shader* m_defaultshader;

public:
	IDraww(Shaders::ColorObject::Shader* shader = NULL) {
		if(shader == NULL) {
			shader = getDefaultShader();
		}
		shader = m_defaultshader;
	}
	Shaders::ColorObject::Shader* IDraww::getDefaultShader() {
		GlobalStart();
		return NULL;
	}
	virtual void GlobalStart() {
		m_defaultshader = 0;
	}
	virtual void GlobalEnd() {
		delete m_defaultshader;
	}
};