#include "Shapes.h"
#include "../App/class.Direct.h"
#include "../../Global.h"
using namespace Shapes;

/*
void Base::NajdiBod(vec &o) {
	// Todo
	//DrawLine(getOrigin(), o, color);
}
*/

void Base::DrawSustavu(const float size) {
	sustava.Render(getOrigin(), vec(20.f*size, 20.f*size, 20.f*size) );
}
int Base::Test(Ray *b) {
	float distance;
	return Test(b, &distance);
}
int Base::Test(Ray *b, vec* HitVectorFromRay) {
	float distance;
	Test(b, &distance);
	if(distance > 0 ) {
        // The primary ray's direction is assumed to be normalized.
        *HitVectorFromRay = b->v;
		HitVectorFromRay->Normalize();
		*HitVectorFromRay *= distance;
		return 1;
    }
	return 0;
}
int Base::Test(MultyShape *b) { 
	return b->Test(b); 
}
AABBMM* Base::toAABBMM() {
	AABBMM *a = new AABBMM();
	a->setSizeMM(getAbsMin(), getAbsMax());
	return a;
}
BBox* Base::toBBox() {
	BBox *a = new BBox();
	a->setSizeMM(getAbsMin(), getAbsMax());
	return a;
} 
Sphere* Base::toSphere() {
	vec origin = getOrigin();
	vec min = origin - getAbsMin();
	return new Sphere(getOrigin(), min.Length());
}