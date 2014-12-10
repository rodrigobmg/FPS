#pragma once

namespace Hud
{

class IFont {
public:
	virtual void BuildVertexArray(void*, char*, float, float) = 0;
};

}