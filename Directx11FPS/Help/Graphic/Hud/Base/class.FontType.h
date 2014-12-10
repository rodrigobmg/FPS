#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include "../../../Base/System/class.File.h"
#include "inter.IFont.h"

namespace Hud
{

class FontType :
	public IFont
{
private:
	struct Type {
		float left, right;
		int size;
	};
	struct VertexType {
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

	Type* m_font;
	const char* m_filename;

	void Initialize();

public:
	FontType(const char* filename);
	~FontType();

	virtual void BuildVertexArray(void*, char*, float, float);
};

}