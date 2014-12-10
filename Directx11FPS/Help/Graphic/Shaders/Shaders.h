#pragma once
#include "../Base/class.HLSL.h"
#include "../Base/class.Texture.h"
#include "../../Math/Math.h"
// #define HLSL HLSL<Features>

namespace Shaders
{

namespace ColorVertex 
{
	struct Features {
		MatrixList matrix;
	};

	class Shader : public HLSL<Features>
	{
	public:
		void Render();
		void Initialize();
	};
}

namespace ColorObject
{
	struct Features {
		MatrixList matrix;
		D3DXVECTOR4 color;
	};

	class Shader : public HLSL<Features>
	{
	public:
		void Render();
		void Initialize();
	};
}

namespace Texture 
{
	struct Features {
		::Texture texture;
		MatrixList matrix;
	};

	class Shader : public HLSL<Features>
	{
	public:
		void Render();
		void Initialize();
	};
}

namespace Light
{
	struct Features : public Texture::Features {
		struct Light { // Pozor zalezi na poradi
			D3DXVECTOR4 diffuseColor;
			D3DXVECTOR3 lightDirection; 
			float padding;
		};

		Light light;
	};

	class Shader : public HLSL<Features>
	{
	public:
		void Initialize();
		void Render();
	};
}

namespace Font
{
	struct Features {
		MatrixList matrix;
		::Texture texture;
		D3DXVECTOR4 color;
	};

	class Shader : public HLSL<Features>
	{
	public:
		void Initialize();
		void Render();
	};
}

namespace Terrain
{	
	struct Features {
		struct Light {
			D3DXVECTOR4 ambientColor;
			D3DXVECTOR4 diffuseColor;
			D3DXVECTOR3 lightDirection;
			float padding;
		};
		
		MatrixList matrix;
		::Texture texture;
		Light light;
	};

	class Shader : public HLSL<Features>
	{
	public:
		void Initialize();
		void Render();
	};
}


}
