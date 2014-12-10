#pragma once
#include "class.Base.h"
#include "../Map/class.Terrain.h"

namespace Camera 
{

class ITerrain : 
	virtual public Base 
{
public:
	void ApplyHeight(Terrain *terrain, float fix = 2.0f) {
		if(y == NULL) return;
		
		// Get the current position of the camera.
		vec position = GetPosition();

		// Get the height of the triangle that is directly underneath the given camera position.
		if(terrain->GetHeight(position)) {
			position.y += fix;
			// If there was a triangle under the camera then position the camera just above it by two units.
			m_Camera->SetPosition(position);
		}
	}
};


}