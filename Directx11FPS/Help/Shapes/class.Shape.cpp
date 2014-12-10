#include "Shapes.h"
#include "../App/class.Direct.h"
#include "../../Global.h"
using namespace Shape;

void Base::DrawLine(vec &a, vec &b, vec4 color) {
	//gDirect->DrawLine(a, b, color);
}
void Base::DrawVertex(Vertex *list, int count) {	
	//gDirect->DrawVertex(list, count);
}
void Base::NajdiBod(vec &o) {
	DrawLine(getOrigin(), o, color);
}
void Base::DrawSustavu(float size) {
	vec v, o = getOrigin();
	v.x = o.x;
	v.y = o.y + size;
	v.z = o.z;
	//DrawLine(o, v, D3DCOLOR_ARGB(255, 255, 0, 0) );
	v.x = o.x;
	v.y = o.y + size;
	v.z = o.z;
	//DrawLine(o, v, D3DCOLOR_ARGB(255, 0, 255, 0) );
	v.x = o.x;
	v.y = o.y;
	v.z = o.z + size;
	//DrawLine(o, v, D3DCOLOR_ARGB(255, 0, 0, 255) );
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