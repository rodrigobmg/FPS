#include "Shapes.h"
using namespace Shapes;

void Capsule::DrawInicialize() {

}
void Capsule::Drawer() {
	// TODO
}
void Capsule::ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride){
	// TODO
}
void  Capsule::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	// TODO
}
int Capsule::Test(AABB *b) {
	return 0; // TODO
}
int Capsule::Test(AABBMM *b){
	return 0; // TODO
}
int Capsule::Test(Ray *b, float *distance){
	return 0; // TODO
}
int Capsule::Test(BBox *b){
	return 0; // TODO
}
int Capsule::Test(Sphere *b){
	return 0; // TODO
}
int Capsule::Test(vec *bod){
	return 0; // TODO
}
int Capsule::Test(Triangle *b){
	return 0; // TODO
}
int Capsule::Test(Capsule *b){
	return 0; // TODO
}
int Capsule::Test(Valec *b){
	return 0; // TODO
}
int Capsule::Test(Frustum *b){
	return 0; // TODO
}
vec Capsule::getAbsMin() {
	return 0; // TODO
}
vec Capsule::getAbsMax() {
	return 0; // TODO
}