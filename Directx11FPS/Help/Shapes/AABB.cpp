#include "Shapes.h"
#include "../../Global.h"
using namespace Shapes;

AABB::AABB() 
{

}
void AABB::DrawInicialize() {
	const UINT indices[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7
    };
	
	shape.CreateVStructure(8);
	shape.CreateIStructure(indices, 24);
	shape.CreateDVBuffer();
	shape.CreateSIBuffer();
}

void AABB::setSizeMM(vec &absmin, vec &absmax) {
	Center = (absmin + absmax) * 0.5f;
	Extents = absmax - Center;
}
void AABB::setSize(vec &origin, vec &extend) {
	Center = origin;
	Extents = extend;
}
void  AABB::setSize(vec &origin, vec &min, vec &max) {
	Center = origin;
	Extents = (-min + max) * 0.5f;
}
void AABB::setSize(vec &origin2, float r) {
	Center = origin2;
	Extents.x = Extents.y = Extents.z = r;
}
vec AABB::getOrigin() { 
	return Center;
}
void AABB::setOrigin(vec &a) {
	Center = a;
}
vec AABB::getPoint(int i) {
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
vec* AABB::getPoints()
{
	vec *body = new vec[8];

	// Delime ...				// bod A
	body[0].x = Center.x + Extents.x;
	body[0].y = Center.y - Extents.y;
	body[0].z = Center.z + Extents.z;
								// bod B
	body[1].x = Center.x - Extents.x;
	body[1].y = Center.y - Extents.y;
	body[1].z = Center.z - Extents.z;	// POZOR NA CHYBY !
								// bod C
	body[2].x = Center.x - Extents.x;
	body[2].y = Center.y - Extents.y;
	body[2].z = Center.z + Extents.z;
								// bod D
	body[3].x = Center.x + Extents.x;
	body[3].y = Center.y - Extents.y;
	body[3].z = Center.z - Extents.z;
								// bod E
	body[4].x = Center.x + Extents.x;
	body[4].y = Center.y + Extents.y;
	body[4].z = Center.z + Extents.z;
								// bod F
	body[5].x = Center.x - Extents.x;
	body[5].y = Center.y + Extents.y;
	body[5].z = Center.z - Extents.z;	// POZOR NA CHYBY !
								// bod G
	body[6].x = Center.x - Extents.x;
	body[6].y = Center.y + Extents.y;
	body[6].z = Center.z + Extents.z;
								// bod H
	body[7].x = Center.x + Extents.x;
	body[7].y = Center.y + Extents.y;
	body[7].z = Center.z - Extents.z;
	// POZOR NA CHYBY !
	return body;
}
vec AABB::getAbsMin() {
	return Center - Extents;
}
vec AABB::getAbsMax() {
	return Center + Extents;
}
void AABB::ComputeFromPoints(UINT Count, const vec* pPoints, UINT Stride) {
	XNA::ComputeBoundingAxisAlignedBoxFromPoints(this, Count, pPoints, Stride);
}
void AABB::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	XNA::TransformAxisAlignedBox(this, this, Scale, vec42XMVECTOR(Rotation), vec42XMVECTOR(Translation));
}
int AABB::Test(vec *bod) { 
	return XNA::IntersectPointAxisAlignedBox(vec2XMVECTOR(bod), this);
}
int AABB::Test(Ray *a, float *distance) {
	return XNA::IntersectRayAxisAlignedBox(vec2XMVECTOR(&(a->getA())), vec2XMVECTOR(&(a->getV())), this, distance);
}
int AABB::Test(AABB *b) {
	return XNA::IntersectAxisAlignedBoxAxisAlignedBox(this, b);
}
int AABB::Test(BBox *b) {
	return XNA::IntersectAxisAlignedBoxOrientedBox(this, b);
}
/*int AABB::Test(Frustum *b) {
	return 0; //return XNA::IntersectAxisAlignedBoxFrustum(this, b);
}*/
int AABB::Test(Sphere *b) {
	return XNA::IntersectSphereAxisAlignedBox(b, this);
}
int AABB::Test(Plane *b) {
	return 0; // TODO
}
int AABB::Test(Capsule *b) {
	return 0; // TODO
}
int AABB::Test(Frustum *b) {
	return 0; // TODO
}
int AABB::Test(Valec *b) {
	return 0; // TODO
}
int AABB::Test(AABBMM *b) {
	return 0; // TODO
}
int AABB::Test(Triangle *b) {
	return b->Test(this);
}
void AABB::Drawer() {
    XMMATRIX matWorld = XMMatrixScaling( Extents.x, Extents.y, Extents.z );
    XMVECTOR position = XMLoadFloat3( &getOrigin() );
    matWorld.r[3] = XMVectorSelect( matWorld.r[3], position, XMVectorSelectControl( 1, 1, 1, 0 ) );

	const XMVECTOR verts[8] = {
        { -1, -1, -1, 0 },
        { 1, -1, -1, 0 },
        { 1, -1, 1, 0 },
        { -1, -1, 1, 0 },
        { -1, 1, -1, 0 },
        { 1, 1, -1, 0 },
        { 1, 1, 1, 0 },
        { -1, 1, 1, 0 }
    };

    // copy to vertex buffer
    for( DWORD i=0; i < 8; ++i ) {
        XMVECTOR v = XMVector3Transform( verts[i], matWorld );
        XMStoreFloat3( &shape.Vertex()[i].position, v );
    }

	shape.SetVBuffer();
}

/*
int AABB::Test(vec *bod) { 
    return	( 
			(absmin.x < bod->x && bod->x < absmax.x)
			&&
			(absmin.y < bod->y && bod->y < absmax.y)
			&&
			(absmin.z < bod->z && bod->z < absmax.z)
		);
}
*/