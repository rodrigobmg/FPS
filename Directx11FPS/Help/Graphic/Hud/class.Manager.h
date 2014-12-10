#pragma once
#include "Base/class.Driver.h"
#include "Base/class.FontType.h"
#include "Base/class.Text.h"
#include "../../Base/Structures/class.CMap.h"
#include "../../Base/String.h"

namespace Hud
{

class Manager : 
	public Driver
{
private:

	class FontLibrary : public std::CMap<const char*, FontType*> {
	protected:
		virtual FontType* New(const char* type) {
			return new FontType( std::string("../Media/Structures/"+ std::string(type) ).c_str() );
		}
	};

	class ShaderLibrary : public std::CMap<const char*, Shaders::Font::Shader*>  {
	public:
		Shaders::MatrixList m_mat;

	protected:
		virtual Shaders::Font::Shader* New(const char* type) {
			Shaders::Font::Shader* shader = new Shaders::Font::Shader();
			shader->Initialize();
			shader->Features()->texture.Add( s2ws( std::string("../Media/Textures/"+ std::string(type) ) ).c_str() );
			shader->Features()->color = vec4(1.0, 0.0, 0.0, 1.0);
			shader->Features()->matrix = m_mat;
			return shader;
		}
	};

	FontLibrary m_fonts;
	ShaderLibrary m_shaders;

public:
	Manager() {		

	}
	~Manager() {
	}
	void Initialize(Screen& info, Shaders::MatrixList& mat) {
		m_shaders.m_mat = mat;
		Driver::Initialize(info);
	}

	Text* Request(const char* type, const char* shader) {
		return new Hud::Text( m_fonts.Request(type), m_shaders.Request(shader) );
	}
};



}