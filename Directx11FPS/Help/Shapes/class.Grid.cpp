#include "Shapes.h"
#include "../../Global.h"
using namespace Shapes;

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
void AABB::Draw(vec4 color)
{
	vec *bod = getPoints();
	DrawLine(bod[0], bod[1], color);
	DrawLine(bod[0], bod[4], color);
	DrawLine(bod[1], bod[2], color);
	DrawLine(bod[1], bod[5], color);
	DrawLine(bod[2], bod[3], color);
	DrawLine(bod[2], bod[6], color);
	DrawLine(bod[3], bod[0], color);
	DrawLine(bod[3], bod[7], color);

	DrawLine(bod[4], bod[5], color);
	DrawLine(bod[5], bod[6], color);
	DrawLine(bod[6], bod[7], color);
	DrawLine(bod[7], bod[4], color);
	SAFE_DELETE_ARRAY(bod);
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

//--------------------------------------------------------------------------------------
// Simple draw operations
//--------------------------------------------------------------------------------------
void DrawGrid( IDirect3DDevice9* pd3dDevice, const XMFLOAT3& XAxis, const XMFLOAT3& YAxis,
               const XMFLOAT3& Origin, int iXDivisions, int iYDivisions, D3DCOLOR Color )
{
    HRESULT hr;

    iXDivisions = max( 1, iXDivisions );
    iYDivisions = max( 1, iYDivisions );

    // build grid geometry
    INT iLineCount = iXDivisions + iYDivisions + 2;
    assert( (2*iLineCount) <= MAX_VERTS );

    XMFLOAT3* pLines = 0;
    V( g_pVB->Lock( 0, 0, (void**)&pLines, D3DLOCK_DISCARD ) )

    XMVECTOR vX = XMLoadFloat3( &XAxis );
    XMVECTOR vY = XMLoadFloat3( &YAxis );
    XMVECTOR vOrigin = XMLoadFloat3( &Origin );

    for( INT i = 0; i <= iXDivisions; i++ )
    {
        FLOAT fPercent = ( FLOAT )i / ( FLOAT )iXDivisions;
        fPercent = ( fPercent * 2.0f ) - 1.0f;
        XMVECTOR vScale = XMVectorScale( vX, fPercent );
        vScale = XMVectorAdd( vScale, vOrigin );
        XMStoreFloat3( &pLines[ ( i * 2 ) ], XMVectorSubtract( vScale, vY ) );
        XMStoreFloat3( &pLines[ ( i * 2 ) + 1 ], XMVectorAdd( vScale, vY ) );
    }

    INT iStartIndex = ( iXDivisions + 1 ) * 2;
    for( INT i = 0; i <= iYDivisions; i++ )
    {
        FLOAT fPercent = ( FLOAT )i / ( FLOAT )iYDivisions;
        fPercent = ( fPercent * 2.0f ) - 1.0f;
        XMVECTOR vScale = XMVectorScale( vY, fPercent );
        vScale = XMVectorAdd( vScale, vOrigin );
        XMStoreFloat3( &pLines[ ( i * 2 ) + iStartIndex ], XMVectorSubtract( vScale, vX ) );
        XMStoreFloat3( &pLines[ ( i * 2 ) + 1 + iStartIndex ], XMVectorAdd( vScale, vX ) );
    }

    V( g_pVB->Unlock() )

    // draw grid
    D3DXCOLOR clr = Color;
    g_pEffect9->SetFloatArray( g_Color, clr, 4 );
    g_pEffect9->CommitChanges();
    pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, iLineCount );
}