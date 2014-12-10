#include "Shapes.h"
#include <assert.h>
using namespace Shapes;

void Plane::DrawInicialize() {

}
void Plane::Drawer() {
	// TODO
}
void Plane::ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride){
	// TODO
}
void  Plane::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	// TODO
}
int Plane::Test(AABB *b) {
	return 0; // TODO
}
int Plane::Test(AABBMM *b){
	return 0; // TODO
}
int Plane::Test(Ray *b, float *distance){
	return 0; // TODO
}
int Plane::Test(BBox *b){
	return 0; // TODO
}
int Plane::Test(Sphere *b){
	return 0; // TODO
}
int Plane::Test(vec *bod){
	return 0; // TODO
}
int Plane::Test(Triangle *b){
	return 0; // TODO
}
int Plane::Test(Plane *b){
	return 0; // TODO
}
int Plane::Test(Valec *b){
	return 0; // TODO
}
int Plane::Test(Frustum *b){
	return 0; // TODO
}
AABBMM* Plane::toAABBMM(){
	assert(0);
	return NULL;
}
BBox* Plane::toBBox(){
	assert(0);
	return NULL;
}
Sphere* Plane::toSphere(){
	assert(0);
	return NULL;
}
vec Plane::getAbsMin() {
	return 0; // TODO
}
vec Plane::getAbsMax() {
	return 0; // TODO
}