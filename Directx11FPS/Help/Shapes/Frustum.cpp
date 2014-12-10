#include "Shapes.h"
#include "../../Global.h"
using namespace Shapes;

Frustum::Frustum() {

}

void Frustum::DrawInicialize() {
	const UINT indices[] = {
		0, 1, 2, 
		3, 0, 4, 
		1, 5, 2,
		6, 3, 7, 
		4, 5, 6, 
		7, 4
	};
	
	shape.CreateVStructure(8);
	shape.CreateIStructure(indices, 17);

	shape.CreateSIBuffer();
	shape.CreateDVBuffer();
}

void Frustum::setOrigin(vec &a) {
	this->Origin = a;
} 
void Frustum::Transform(float Scale, vec4* Rotation, vec4* Translation) {
	// TODO
}
void Frustum::SetPlanes(D3DXPLANE* planes) {
	for(USHORT i=0; i < 6; i++) m_planes[i] = planes[i];
}
D3DXPLANE* Frustum::GetPlanes() {
	return m_planes;
}
vec* Frustum::getPoints() {
	vec* points = NULL;
	//points = new vec[8];
	// TODO


	return points;
}
vec Frustum::getPoint(int i) { 
	vec* points = getPoints();
	vec point = points[i];
	SAFE_DELETE_ARRAY(points)
	return point;
}
void Frustum::Transform(float Scale, vec Rotation, vec Translation) {
	XNA::TransformFrustum(this, this, Scale, vec2XMVECTOR(&Rotation), vec2XMVECTOR(&Translation));
}
int Frustum::Test(vec *bod) { 
	return XNA::IntersectPointFrustum( vec2XMVECTOR(bod) , this);
}
int Frustum::Test(Frustum *b) {
	return XNA::IntersectFrustumFrustum( this, b);
}
vec Frustum::getOrigin() { 
	return this->Origin;
}
void Frustum::ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride){
	// TODO
}
int Frustum::Test(AABB *box) {
	return XNA::IntersectAxisAlignedBoxFrustum(box, this);
}
int Frustum::Test(AABBMM *c){
	return 0;
}
int Frustum::Test(Ray *b, float *distance){
	return 0; // TODO
}
int Frustum::Test(BBox *b){
	return XNA::IntersectOrientedBoxFrustum(b, this);
}
int Frustum::Test(Sphere *b){
	return XNA::IntersectSphereFrustum(b, this);
}
int Frustum::Test(Triangle *b){
	return 0; // TODO
}
int Frustum::Test(Plane *b){
	// IntersectFrustumPlane( const Frustum* pVolume, FXMVECTOR Plane )
	return 0; // TODO
}
int Frustum::Test(Valec *b){
	return 0; // TODO
}
int Frustum::Test(Capsule *b){
	return 0; // TODO
}
vec Frustum::getAbsMin() {
	// Prehladavaj a hladaj MIN
	return 0; // TODO
}
vec Frustum::getAbsMax() {
	return 0; // TODO
}
void Frustum::ComputeFromProjection(XMMATRIX* pProjection ) {
	XNA::ComputeFrustumFromProjection(this, pProjection); // vynasobyt potom
}
void Frustum::Drawer() 
{
    // compute corner points
    XMVECTOR Origin = vec2XMVECTOR( &getOrigin() );
	
	// TODO: near, far sa nepouzivaju lebo frustum bolo ine pouzite
    FLOAT Near = this->Near;
    FLOAT Far = this->Far;
    FLOAT RightSlope = this->RightSlope;
    FLOAT LeftSlope = this->LeftSlope;
    FLOAT TopSlope = this->TopSlope;
    FLOAT BottomSlope = this->BottomSlope;

	XMFLOAT3 vertex[8];
    vertex[0] = XMFLOAT3( RightSlope * Near, TopSlope * Near, Near );
    vertex[1] = XMFLOAT3( LeftSlope * Near, TopSlope * Near, Near );
    vertex[2] = XMFLOAT3( LeftSlope * Near, BottomSlope * Near, Near );
    vertex[3] = XMFLOAT3( RightSlope * Near, BottomSlope * Near, Near );

    vertex[4] = XMFLOAT3( RightSlope * Far, TopSlope * Far, Far );
    vertex[5] = XMFLOAT3( LeftSlope * Far, TopSlope * Far, Far );
    vertex[6] = XMFLOAT3( LeftSlope * Far, BottomSlope * Far, Far );
    vertex[7] = XMFLOAT3( RightSlope * Far, BottomSlope * Far, Far );
	
    XMVECTOR Orientation = vec42XMVECTOR( &this->Orientation );
    XMMATRIX Mat = XMMatrixRotationQuaternion( Orientation );
    for( UINT i = 0; i < 8; i++ ) {
        XMVECTOR Result = XMVector3Transform( XMLoadFloat3( &vertex[i] ), Mat );
        Result = XMVectorAdd( Result, Origin );
        XMStoreFloat3( &shape.Vertex()[i].position, Result );
    }

    // copy to vertex buffer
    shape.SetVBuffer();
}