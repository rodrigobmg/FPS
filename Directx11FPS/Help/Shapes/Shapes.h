#pragma once
#include "class.Base.h"
#include "../Base/Structures/class.DynArray.h"

namespace Shapes {

class Ray : public Base
{
protected:
	vec a, v;

public:
	SHAPE_INTERFACE()

	void SetA(vec &a);
	void SetV(vec &v);
	vec getA();
	vec getV();

	vec EndOrigin();
	vec getPoint(int i); // a, koniec, stred
	vec getOrigin();
	void setOrigin(vec &a);
}; 

class Valec : public Ray
{
	float r; // vzdialenost od tej priamky

public:
	Valec() {}
	SHAPE_INTERFACE()
	
	float getR();
	void setR(float r);
	Ray* toRay();
}; 

class Capsule : public Valec
{
	float r1; // vzdialenost od horneho konca
	float r2; // vzdialenost od dolneho konca

public:
	void SetSize(float r);
	SHAPE_INTERFACE()
}; 

class Sphere : public Base, protected XNA::Sphere {	
public:
	Sphere();
	Sphere(vec &origin, float r) { 
		setSize(origin, r); 
		Sphere();
	}
	SHAPE_INTERFACE()

	void setSize(vec &a, float x);
	vec getOrigin() { 
		return Center;
	}
	float getRadius() { return this->Radius; }
	void setRadius(float r) { this->Radius = r; }
	void setOrigin(vec &a) {Center = a; } 
	Sphere* toSphere();

private:
	DWORD dwRingSegments;
	void DrawRing(const DWORD id, const XMFLOAT3& Origin, const XMFLOAT3& MajorAxis, const XMFLOAT3& MinorAxis);
};

class AABB : public Base, protected XNA::AxisAlignedBox
{
public: 
	AABB();
	SHAPE_INTERFACE()

	void setSizeMM(vec &absmin, vec &absmax);
	void setSize(vec &origin, vec &extend);
	void setSize(vec &origin, vec &min, vec &max);
	void setSize(vec &origin, float r);

	void setOrigin(vec &a);
	vec getOrigin();
	vec getPoint(int i);
	vec* getPoints();
};

class AABBMM : public Base
{
protected:
	vec absmin;
	vec absmax;

public: 
	AABBMM();
	SHAPE_INTERFACE()

	void setSizeMM(vec &absmin, vec &absmax);
	void setSize(vec &origin, vec &extend);
	void setSize(vec &origin, vec &min, vec &max);
	void setSize(vec &origin, float r);

	void setOrigin(vec &a);
	vec getOrigin();
	vec getPoint(int i);
	vec* getPoints();
};

class BBox : public Base, protected XNA::OrientedBox
{
public: 
	BBox() { };
	SHAPE_INTERFACE()

	void setSizeMM(vec &absmin, vec &absmax);
	void setSize(vec &origin, vec &extend);
	void setSize(vec &origin, vec &min, vec &max);
	void setSize(vec &origin, float r);

	void setOrigin(vec &a);
	vec getOrigin();
	vec getPoint(int i);
	vec* getPoints();

	BBox*  toBBox();
};

class Frustum : public Base, protected XNA::Frustum
{
protected:
	D3DXPLANE m_planes[6];
	int CheckAABBMM(vec &absmin, vec &absmax);

public:
	Frustum();
	SHAPE_INTERFACE()

	void ComputeFromProjection(XMMATRIX* pProjection );
	void Transform(float Scale, vec Rotation, vec Translation);
	
	void SetPlanes(D3DXPLANE* pPlanes);
	D3DXPLANE* GetPlanes();
	vec* getPoints();
	vec getOrigin();
	virtual vec getPoint(int i);
	void setOrigin(vec &a);
};


class MultyShape : public Base {
	DynArray<Base*> objects;
	DynArray<vec*> offsets;
	vec origin;

public:
	vec getPoint(int i);
	void setColor(vec4 x);
	void setOrigin(vec &a);
	vec getOrigin() { return origin; }
	SHAPE_INTERFACE()

	AABBMM* toAABBMM();
	BBox* toBBox();
	Sphere* toSphere();

	int Test(MultyShape *b);
	Base* getObject(int i);
};

class Triangle : public Base {
	vec body[3];

public:
	SHAPE_INTERFACE()

	void setSize(vec a, vec b, vec c);
	vec getPoint(int i);
	vec getOrigin();
	void setOrigin(vec &a);

	AABBMM* toAABBMM();
	BBox* toBBox();
	Sphere* toSphere();
};

class Plane : public D3DXPLANE {
public:
	SHAPE_INTERFACE()

	vec getPoint(int i);
	vec getOrigin();
	void setOrigin(vec &a);
	
	AABBMM* toAABBMM();
	BBox* toBBox();
	Sphere* toSphere();
};


}
