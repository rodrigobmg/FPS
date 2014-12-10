#pragma once
#include "class.Base.h"
#include "../../Shapes/Shapes.h"
#include "../../App/class.Direct.h"
extern Direct* gDirect;

namespace Camera {

class IFrustum : virtual public Base {
public:
	inline Shapes::Frustum* getFrustum() { return &m_frustum; }

protected:
	void BuildFrustum() {
		m_frustum.ComputeFromProjection((XMMATRIX*) gDirect->GetProjectionMatrix());
	}
	void SetFrustum() {
		vec rotation = Base::GetRotation();
		vec position = Base::GetPosition();	
		m_frustum.setOrigin( position );
		if(rotation == 0.0f) {			
			m_frustum.setOrigin( position );
		} else {
			//m_frustum.Transform(1.0f, rotation, position); // TODO rotation na quaterniony
		}
	}

private:
	Shapes::Frustum m_frustum;
};



}