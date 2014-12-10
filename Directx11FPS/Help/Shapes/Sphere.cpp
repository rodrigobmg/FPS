#include "Shapes.h"
using namespace Shapes;

Sphere::Sphere() {

}

void Sphere::DrawInicialize() {
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

	dwRingSegments = 32;
	shape.CreateVStructure( (dwRingSegments+1)*3 );
	shape.CreateIStructure(indices, 24);
	shape.CreateDVBuffer();
	shape.CreateSIBuffer();
}
void Sphere::setSize(vec &a, float x) {
	Center = a;
	Radius = x;
}
void Sphere::ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride) {
	XNA::ComputeBoundingSphereFromPoints(this, Count, pPoints, Stride);
}
void Sphere::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	XNA::TransformSphere(this, this, Scale, vec42XMVECTOR(Rotation), vec42XMVECTOR(Translation));
}

int Sphere::Test(vec *bod) {
	return XNA::IntersectPointSphere(vec2XMVECTOR(bod), this);
}
int Sphere::Test(Ray *a, float *distance) {
	return XNA::IntersectRaySphere(vec2XMVECTOR(&a->getA()), vec2XMVECTOR(&a->getV()), this, distance);
}
int Sphere::Test(Sphere *b) {
	return XNA::IntersectSphereSphere(this, b);
}
int Sphere::Test(AABB *b) {
	return XNA::IntersectSphereAxisAlignedBox(this, b);
}
int Sphere::Test(BBox *b) {
	return XNA::IntersectSphereOrientedBox(this, b);
}
int Sphere::Test(Triangle *b) {
	return 0;
}
int Sphere::Test(Plane *b) {
	return 0; // TODO
}
int Sphere::Test(Capsule *b) {
	return 0; // TODO
}
int Sphere::Test(Valec *b) {
	return 0; // TODO
}
int Sphere::Test(AABBMM *b) {
	return 0; // TODO
}
int Sphere::Test(Frustum *b) {
	return 0;//return XNA::IntersectSphereFrustum(this, b);
}

Sphere* Sphere::toSphere() {
	return new Sphere(Center, Radius);
}
vec Sphere::getAbsMin() {
	return Center - vec(Radius);
}
vec Sphere::getAbsMax() {
	return Center + vec(Radius);
}

void Sphere::DrawRing(const DWORD id, const XMFLOAT3& Origin, const XMFLOAT3& MajorAxis, const XMFLOAT3& MinorAxis)
{
	XMVECTOR vOrigin = XMLoadFloat3( &Origin );
    XMVECTOR vMajor = XMLoadFloat3( &MajorAxis );
    XMVECTOR vMinor = XMLoadFloat3( &MinorAxis );

    FLOAT fAngleDelta = XM_2PI / ( float )dwRingSegments;
    // Instead of calling cos/sin for each segment we calculate
    // the sign of the angle delta and then incrementally calculate sin
    // and cosine from then on.
    XMVECTOR cosDelta = XMVectorReplicate( cosf( fAngleDelta ) );
    XMVECTOR sinDelta = XMVectorReplicate( sinf( fAngleDelta ) );
    XMVECTOR incrementalSin = XMVectorZero();
    static const XMVECTOR initialCos = {
        1.0f, 1.0f, 1.0f, 1.0f
    };
    XMVECTOR incrementalCos = initialCos;
    for( DWORD i = 0; i < dwRingSegments; i++ )
    {
        XMVECTOR Pos;
        Pos = XMVectorMultiplyAdd( vMajor, incrementalCos, vOrigin );
        Pos = XMVectorMultiplyAdd( vMinor, incrementalSin, Pos );
        XMStoreFloat3( ( XMFLOAT3* )&shape.Vertex()[id*dwRingSegments+i], Pos );
        // Standard formula to rotate a vector.
        XMVECTOR newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
        XMVECTOR newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;
        incrementalCos = newCos;
        incrementalSin = newSin;
    }
    shape.Vertex()[ (id+1)*dwRingSegments ] = shape.Vertex()[ id*dwRingSegments ];
}

void Sphere::Drawer() {
    XMFLOAT3 Origin = vec2XMFLOAT3(&getOrigin());
    DrawRing(0, Origin, XMFLOAT3( Radius, 0, 0 ), XMFLOAT3( 0, 0, Radius ));
    DrawRing(1, Origin, XMFLOAT3( Radius, 0, 0 ), XMFLOAT3( 0, Radius, 0 ));
	DrawRing(2, Origin, XMFLOAT3( 0, Radius, 0 ), XMFLOAT3( 0, 0, Radius ));
	shape.SetVBuffer();
}