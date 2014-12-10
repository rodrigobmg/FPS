#pragma once
#include <d3d11.h>
#include <d3dx11tex.h>

#include "../../../../Global.h"
#include "../../../App/class.Direct.h"
#include "class.FontType.h"
#include "../../Shaders/Shaders.h"
#include "../../Structures/inter.IBase.h"
#include "../../../Math/class.vec.h"
#include "class.Item.h"


namespace Hud
{

struct VertexType {
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
};

class Text : 
	public Item,
	protected Structures::IBase<VertexType, UINT>
{
private:
	void Preset(UINT max);
	void Initialize(UINT max);

	Hud::FontType* m_font;
	Shaders::Font::Shader* m_fontshader;
	D3DXVECTOR4 m_color;
	char* m_text;
	vec2 m_position;

protected:
	virtual void Render();

public:
	Text() {
		m_text = NULL;
		m_font = NULL;
		m_fontshader = NULL;
	}
	Text(FontType* font, Shaders::Font::Shader* shader) {
		m_text = NULL;
		m_font = font;
		m_fontshader = shader;
	}

	void Set(char* text, vec2 position);	
	char* getText() { return m_text; }
	vec2 getPosition() { return m_position; }
	UINT getLength();

	D3DXVECTOR4 getColor() { return m_color; }
	void setColor(D3DXVECTOR4 color) { m_color = color; }

	FontType* getFont() { return m_font; }
	void setFont(FontType* font) { m_font = font; }

	Shaders::Font::Shader* getFontShader() { return m_fontshader; }
	void setFontShader(Shaders::Font::Shader* font) { m_fontshader = font; }

	friend std::ostream& operator<<(std::ostream& out, Text& v){
		return out << v.m_text;
	}
};


}