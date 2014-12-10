#include "Shapes.h"
using namespace Shapes;

void MultyShape::DrawInicialize() {

}
vec MultyShape::getPoint(int i) {
	return objects.GetAt(i)->getOrigin();
}
Base* MultyShape::getObject(int i) {
	return objects.GetAt(i);
}
void MultyShape::setColor(vec4 x) {
	for(UINT i=0; i < objects.GetSize(); i++) {
		objects.GetAt(i)->setColor(x);
	}
}
void MultyShape::setOrigin(vec &a) {
	if(origin != a) {
		origin = a;
		vec *offest;
		for(UINT i=0; i < objects.GetSize(); i++) {
			offest = offsets.GetAt(i);
			objects.GetAt(i)->setOrigin(origin+*offest);
		}
	}
}
void MultyShape::Drawer() {
	for(UINT i=0; i < objects.GetSize(); i++) {
		objects.GetAt(i)->Draw();
	}
}
int MultyShape::Test(AABB *b) {
	for(UINT i=0; i < objects.GetSize(); i++) {
		if(objects.GetAt(i)->Test(b) > 0) return i+1; 
	}
	return 0;
}
int MultyShape::Test(Ray *b, float *distance){
	for(UINT i=0; i < objects.GetSize(); i++) {
		if(objects.GetAt(i)->Test(b) > 0) return i+1; 
	}
	return 0;
}
int MultyShape::Test(BBox *b){
	for(UINT i=0; i < objects.GetSize(); i++) {
		if(objects.GetAt(i)->Test(b) > 0) return i+1; 
	}
	return 0;
}
int MultyShape::Test(Sphere *b){
	for(UINT i=0; i < objects.GetSize(); i++) {
		if(objects.GetAt(i)->Test(b) > 0) return i+1; 
	}
	return 0;
}
int MultyShape::Test(vec *b){
	for(UINT i=0; i < objects.GetSize(); i++) {
		if(objects.GetAt(i)->Test(b) > 0) return i+1; 
	}
	return 0;
}
int MultyShape::Test(Plane *b) {
	return 0; // TODO
}
int MultyShape::Test(MultyShape *b) { 
	return 0; 
}
AABBMM* MultyShape::toAABBMM(){
	// TODO
	AABBMM *shape = new AABBMM;
	vec absmin;
	vec absmax;
	for(UINT i=0; i < objects.GetSize(); i++) {
		//if(objects.GetAt(i)->Test(b) > 0) return i+1; 
	}
	return shape;
}
BBox* MultyShape::toBBox(){
	// TODO
	BBox *shape = new BBox;
	return shape;
}
Sphere* MultyShape::toSphere(){
	// TODO
	Sphere *shape = new Sphere;
	return shape;
}
void MultyShape::ComputeFromPoints(UINT Count, const vec* pPoints, UINT Stride) { }