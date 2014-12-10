#pragma once 
//#include "../../Base/class.Module.h"
#include "inter.IFrustum.h"
#include "inter.IMoveable.h"

namespace Camera 
{

class Camera :  
	public IMoveable, 
	public IFrustum 
	//public Module
{
protected:

public:
	Camera() {
		BuildFrustum();
	}
	void Frame(float delta)  {
		ProccessKeys(delta);
		if(!m_moved) return;
		Base::PrepareMatrix();
		SetFrustum();
	}
};



}
