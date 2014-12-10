#include "Shapes.h"
#include "../Math/xnacollision.h"
#include <assert.h>
using namespace Shapes;

void Ray::DrawInicialize() {

}
void Ray::SetA(vec &aa) {
	this->a = aa;
}
void Ray::SetV(vec &vv) {
	this->v = vv;
}
vec Ray::EndOrigin() {
	return a + v;
}
vec Ray::getPoint(int i) {
	switch(i) {
		case 0: return a;
		case 1: return getOrigin();
		case 2: return EndOrigin();
		default: return NULL; 
	}
}
vec Ray::getOrigin() {
	return a + (v / 2.0f);
}
void Ray::setOrigin(vec &a) {
	vec x = a - getOrigin();
	a = a + x;
	v = v + x;
}
void Ray::Drawer() {
	// TODO
	//DrawLine(a, EndOrigin(), color);
}
vec Ray::getA() {
	return a;
}
vec Ray::getV() {
	return v;
}
int Ray::Test(Ray *b, float *distance) {
	// TODO
	return 0;
}
int Ray::Test(AABB *b) {
	return b->Base::Test(this);
}
int Ray::Test(BBox *b) {
	return b->Base::Test(this);
}
int Ray::Test(Sphere *b){
	return b->Base::Test(this);
}
int Ray::Test(vec *bod){
	// TODO
	return 0;
}
int Ray::Test(Triangle *b){
	// TODO
	return 0;
}
void Ray::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	// TODO
}
void Ray::ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride){
	// TODO
}
int Ray::Test(Plane *b) {
	return 0; // TODO
}
int Ray::Test(Capsule *b) {
	return 0; // TODO
}
int Ray::Test(Valec *b) {
	return 0; // TODO
}
int Ray::Test(AABBMM *b) {
	return 0; // TODO
}
int Ray::Test(Frustum *b) {
	return 0; // TODO
}
vec Ray::getAbsMin() {
	vec x;
	return x;
	// TODO
}
vec Ray::getAbsMax() {
	vec x;
	return x;
	// TODO
}