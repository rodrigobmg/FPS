#include "Shapes.h"
using namespace Shapes;

void Valec::DrawInicialize() {

}
float Valec::getR() {
	return r;
}
void Valec::setR(float r) {
	this->r = r;
}
Ray* Valec::toRay() { 
	Ray *a = new Ray; 
	*a = *this;
	return a;
}

void Valec::Drawer() {

}
void Valec::ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride){
	// TODO
}
void  Valec::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	// TODO
}
int Valec::Test(AABB *b) {
	return 0; // TODO
}
int Valec::Test(AABBMM *b){
	return 0; // TODO
}
int Valec::Test(Ray *b, float *distance){
	return 0; // TODO
}
int Valec::Test(BBox *b){
	return 0; // TODO
}
int Valec::Test(Sphere *b){
	return 0; // TODO
}
int Valec::Test(vec *bod){
	return 0; // TODO
}
int Valec::Test(Triangle *b){
	return 0; // TODO
}
int Valec::Test(Plane *b){
	return 0; // TODO
}
int Valec::Test(Valec *b){
	return 0; // TODO
}
int Valec::Test(Frustum *b){
	return 0; // TODO
}

vec Valec::getAbsMin() {
	return 0; // TODO
}
vec Valec::getAbsMax() {
	return 0; // TODO
}