#pragma once
#include <assert.h>
#include "class.Base.h"
#include "../../Math/Math.h"
#include "../../App/Base/class.Input.h"

namespace Camera 
{

class IMoveable : 
	// Normalny pohyb bez akceleracie, tu neskor mozno implementovat
	// 3D first person camera
	virtual public Base 
{
public:
	IMoveable();
	inline bool hasMoved() { return m_moved; }

protected:
	bool m_moved;
	vec m_pohybspeed;
	vec m_sipkyspeed;
	vec2 m_mousespeed;
	Input *m_input;

	void ProccessKeys(float delta);
private:
	void InvertMouse();
};


}