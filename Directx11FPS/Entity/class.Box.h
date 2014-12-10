#pragma once
#include "class.Base.h"

namespace Entity {

class Box : public Entity::Base
{

public:
	Box() {
		AABBMM *shape = new AABBMM;
		shape->setSize(vec(0.f), 1000.f);
		shape->setColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Entity::IPhysics::setShape(shape);
	}
	virtual void Draw() {
		DebugDraw();
	}
	virtual void setShape(Shapes::Base *s) { 

	}

};


}