#pragma once
#include "../Math/xnacollision.h"
#include "../Graphic/Structures/inter.IBase.h"
#include "../Graphic/Structures/class.Vertex.h"
#include "../Graphic/Shaders/Shaders.h"

extern Shaders::ColorObject::Shader* gIDraw_Defaultshader;

namespace Shapes
{

class IDraw
{
protected:	
	class Sustava : protected Structures::DrawObject {
	public:
		Sustava();
		void Render(vec &origin, const vec &size);

	protected:		
		void Inicialize();
	};

	vec4 color;
	Structures::DrawObject shape;
	Sustava sustava;

	virtual void DrawInicialize() = 0;
	virtual void Drawer() = 0;

public:
	IDraw();
	virtual vec4 getColor() { return color; }
	virtual void setColor(vec4 x) { color = x; }

	// Vykresluj
	void Draw();
	virtual void DrawSustavu(const float size = 1.0) = 0;
	//virtual void NajdiBod(vec &o);

	static void GlobalStart();
	static void GlobalEnd();
};


}
