#include "Shapes.h"
#include "../../Global.h"
using namespace Shapes;

AABBMM::AABBMM() 
{

}
void AABBMM::DrawInicialize() {
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
void AABBMM::setSizeMM(vec &absmin, vec &absmax) {
	this->absmin = absmin;
	this->absmax = absmax;
}
void AABBMM::setSize(vec &origin, vec &extend) {
	this->absmin = origin - extend;
	this->absmax = origin + extend;
}
void  AABBMM::setSize(vec &origin, vec &min, vec &max) {
	this->absmin = origin - min;
	this->absmax = origin + max;
}
void AABBMM::setSize(vec &origin, float r) {
	this->absmin = origin - vec(r, r, r);
	this->absmax = origin + vec(r, r, r);
}
vec AABBMM::getOrigin() { 
	return (absmax + absmin) * 0.5; // Je pozicia nie vektor !!!
}
void AABBMM::setOrigin(vec &a) {
	vec x = a - getOrigin();
	this->absmin += x;
	this->absmax += x;
}
vec AABBMM::getPoint(int i) {
	vec bod;
	vec Center = getOrigin();
	vec Extents = absmax - Center;
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
vec* AABBMM::getPoints()
{
	vec *body = new vec[8];
	vec Center = getOrigin();
	vec Extents = absmax - Center;

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
vec AABBMM::getAbsMin() {
	return absmin;
}
vec AABBMM::getAbsMax() {
	return absmax;
}
void AABBMM::ComputeFromPoints(UINT Count, const vec* pPoints, UINT Stride) {
	
}
void AABBMM::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	
}
int AABBMM::Test(vec *bod) { 
    return	( 
		(absmin.x < bod->x && bod->x < absmax.x)
		&&
		(absmin.y < bod->y && bod->y < absmax.y)
		&&
		(absmin.z < bod->z && bod->z < absmax.z)
	);
}
int AABBMM::Test(Ray *a, float *distance) {
	return 0; // TODO
}
int AABBMM::Test(AABB *b) {
	return 0; // TODO	
}
int AABBMM::Test(AABBMM *b) {
	return 0; // TODO
}
int AABBMM::Test(BBox *b) {
	return 0; // TODO
}
int AABBMM::Test(Frustum *b) {
	return 0; // TODO
}
int AABBMM::Test(Sphere *b) {
	return 0; // TODO
}
int AABBMM::Test(Triangle *b) {
	return 0; // TODO
}
int AABBMM::Test(Plane *b) {
	return 0; // TODO
}
int AABBMM::Test(Capsule *b) {
	return 0; // TODO
}
int AABBMM::Test(Valec *b) {
	return 0; // TODO
}

void AABBMM::Drawer() {
   	vec Center = getOrigin();
	vec Extents = absmax - Center;

	//D3DXMatrixTranslation(&a, sphere->getOrigin().x, sphere->getOrigin().y, sphere->getOrigin().z);
	XMMATRIX matWorld = XMMatrixScaling( Extents.x, Extents.y, Extents.z );
    XMVECTOR position = XMLoadFloat3( &Center );
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