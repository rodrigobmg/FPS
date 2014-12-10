#include "Shapes.h"
#include "../../Global.h"
using namespace Shapes;

void BBox::DrawInicialize() {


}
void BBox::setSizeMM(vec &absmin, vec &absmax) {
	Center = ( absmin + absmax ) * 0.5f;
	Extents = ( absmax - absmin ) * 0.5f;
}
void BBox::setSize(vec &origin, vec &extend) {
	Center = origin;
	Extents = extend;
}
void BBox::setSize(vec &origin, vec &min, vec &max) {
	Center = origin;
	Extents = (-min + max) * 0.5f;
}
void BBox::setSize(vec &origin2, float r) {
	Center = origin2;
	Extents.set(r);
}
vec BBox::getOrigin() { 
	vec x(Center.x, Center.y, Center.z);
	return x;
}
void BBox::setOrigin(vec &a) {
	Center = a;
}
vec BBox::getPoint(int i) {
	vec bod;
	switch(i) {
		case 0: {
			bod.x = Center.x + Extents.x;
			bod.y = Center.y - Extents.y;
			bod.z = Center.z + Extents.z;
			break;
		}
		case 1: {
			bod.x = Center.x - Extents.x;
			bod.y = Center.y - Extents.y;
			bod.z = Center.z - Extents.z;	// POZOR NA CHYBY !
			break;
		}
		case 2: {
			bod.x = Center.x - Extents.x;
			bod.y = Center.y - Extents.y;
			bod.z = Center.z + Extents.z;
			break;
		}
		case 3: {
			bod.x = Center.x + Extents.x;
			bod.y = Center.y - Extents.y;
			bod.z = Center.z - Extents.z;
			break;
		}
		case 4: {
			bod.x = Center.x + Extents.x;
			bod.y = Center.y + Extents.y;
			bod.z = Center.z + Extents.z;
			break;
		}
		case 5: {
			bod.x = Center.x - Extents.x;
			bod.y = Center.y + Extents.y;
			bod.z = Center.z - Extents.z;	// POZOR NA CHYBY !
			break;
		}
		case 6: {
			bod.x = Center.x - Extents.x;
			bod.y = Center.y + Extents.y;
			bod.z = Center.z + Extents.z;
			break;
		}
		case 7: {
			bod.x = Center.x + Extents.x;
			bod.y = Center.y + Extents.y;
			bod.z = Center.z - Extents.z;
			break;
		}
	}
	return bod;
}
vec* BBox::getPoints()
{
	vec *body = new vec[8];
	// TODO
	return body;
}
vec BBox::getAbsMin() {
	return Center - Extents; // TODO Repair
}
vec BBox::getAbsMax() {
	return Center + Extents; // TODO Repair
}
BBox*  BBox::toBBox() {
	BBox *a = new BBox();
	*a = *this;
	return a;
}
void BBox::ComputeFromPoints(UINT Count, const vec* pPoints, UINT Stride) {
	XNA::ComputeBoundingOrientedBoxFromPoints(this, Count, pPoints, Stride);
}
void BBox::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	XNA::TransformOrientedBox(this, this, Scale, vec42XMVECTOR(Rotation), vec42XMVECTOR(Translation));
}
int BBox::Test(vec *bod) { 
	return XNA::IntersectPointOrientedBox(vec2XMVECTOR(bod), this);
}
int BBox::Test(Ray *a, float *distance) {
	return XNA::IntersectRayOrientedBox(vec2XMVECTOR(&(a->getA())), vec2XMVECTOR(&(a->getV())), this, distance);
}
int BBox::Test(Frustum *a) {
	return XNA::IntersectOrientedBoxFrustum(this, a);
}
int BBox::Test(AABB *b) {
	return XNA::IntersectAxisAlignedBoxOrientedBox(b, this);
}
int BBox::Test(BBox *b) {
	return XNA::IntersectOrientedBoxOrientedBox(this, b);
}
int BBox::Test(Sphere *b) {
	return XNA::IntersectSphereOrientedBox(b, this);
}
int BBox::Test(Triangle *b) {
	return b->Test(this);
}
int BBox::Test(Plane *b) {
	return 0; // TODO
}
int BBox::Test(Capsule *b) {
	return 0; // TODO
}
int BBox::Test(Valec *b) {
	return 0; // TODO
}
int BBox::Test(AABBMM *b) {
	return 0; // TODO
}

void BBox::Drawer()
{

}

/*
void DrawObb( IDirect3DDevice9* pd3dDevice, const XNA::OrientedBox& obb, D3DCOLOR Color )
{
    XMMATRIX matWorld = XMMatrixRotationQuaternion( XMLoadFloat4( &obb.Orientation ) );
    XMMATRIX matScale = XMMatrixScaling( obb.Extents.x, obb.Extents.y, obb.Extents.z );
    matWorld = XMMatrixMultiply( matScale, matWorld );
    XMVECTOR position = XMLoadFloat3( &obb.Center );
    matWorld.r[3] = XMVectorSelect( matWorld.r[3], position, XMVectorSelectControl( 1, 1, 1, 0 ) );

    DrawCube( pd3dDevice, matWorld, Color );
}
*/