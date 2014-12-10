#include "Shapes.h"
#include "../Math/xnacollision.h"
using namespace Shapes;

void Triangle::DrawInicialize() {

}
void Triangle::setSize(vec a, vec b, vec c) {
	body[0] = a;
	body[1] = b;
	body[2] = c;
}
void Triangle::Drawer() {
	//DrawLine(body[0], body[1], color);
	//DrawLine(body[1], body[2], color);
	//DrawLine(body[2], body[0], color);
}
vec Triangle::getPoint(int i) {
	return body[i];
}
vec Triangle::getOrigin() {
	// Nieje to uplne tazisko ale staci to takto...
	vec x = (body[0] + body[1]) / 2.0f;
	x = (x + body[2]) / 2.0;
	return x;
}
void Triangle::setOrigin(vec &a) {
	vec x = a - getOrigin();
	body[0] = body[0] + x;
	body[1] = body[1] + x;
	body[2] = body[2] + x;
}
int Triangle::Test(Ray *a, float *distance) {
	return 0;
}
int Triangle::Test(Triangle *b) {
	return XNA::IntersectTriangleTriangle(
		vec2XMVECTOR(&(body[0])), 
		vec2XMVECTOR(&(body[1])), 
		vec2XMVECTOR(&(body[2])), 
		vec2XMVECTOR(&(b->body[0])), 
		vec2XMVECTOR(&(b->body[1])), 
		vec2XMVECTOR(&(b->body[2]))
	);
}
int Triangle::Test(Sphere *b) {
	return XNA::IntersectTriangleSphere(
		vec2XMVECTOR(&body[0]), 
		vec2XMVECTOR(&body[1]), 
		vec2XMVECTOR(&body[2]), 
	b);
}
int Triangle::Test(AABB *b) {
	return XNA::IntersectTriangleAxisAlignedBox(
		vec2XMVECTOR(&body[0]), 
		vec2XMVECTOR(&body[1]), 
		vec2XMVECTOR(&body[2]),
	b);
}
int Triangle::Test(BBox *b) {
	return XNA::IntersectTriangleOrientedBox(
		vec2XMVECTOR(&body[0]), 
		vec2XMVECTOR(&body[1]), 
		vec2XMVECTOR(&body[2]), 
	b);
}
int Triangle::Test(Frustum *b) {
	return XNA::IntersectTriangleFrustum(vec2XMVECTOR(&body[0]), vec2XMVECTOR(&body[1]), vec2XMVECTOR(&body[2]), b);
}
AABBMM* Triangle::toAABBMM() {
	// TODO min a max zistit
	return 0;
}
BBox* Triangle::toBBox() {
	// TODO zistit najlepsiu poziciu
	return 0;
}
Sphere* Triangle::toSphere() {
	Sphere *a = new Sphere();
	//a->setSize(getOrigin(), NajdlchsiaTaznica* 2/3);
	return a;
}
void Triangle::ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride) {
	body[0] = pPoints[0];
	body[1] = pPoints[1];
	body[2] = pPoints[2];
}
/*
void DrawTriangle( IDirect3DDevice9* pd3dDevice, const XMFLOAT3& PointA, const XMFLOAT3& PointB,
                   const XMFLOAT3& PointC, D3DCOLOR Color )
{
    // Copy to vertex buffer
    assert( 4 <= MAX_VERTS );
    XMFLOAT3* pVerts = NULL;
    HRESULT hr;
    V( g_pVB->Lock( 0, 0, (void**)&pVerts, D3DLOCK_DISCARD ) )
    pVerts[0] = PointA;
    pVerts[1] = PointB;
    pVerts[2] = PointC;
    pVerts[3] = PointA;
    V( g_pVB->Unlock() )

    // Draw triangle
    D3DXCOLOR clr = Color;
    g_pEffect9->SetFloatArray( g_Color, clr, 4 );
    g_pEffect9->CommitChanges();
    pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, 0, 3 );
}
*/