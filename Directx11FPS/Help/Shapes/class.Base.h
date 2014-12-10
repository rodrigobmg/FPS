#ifndef H_Shapes
#define H_Shapes
#include "../Math/xnacollision.h"
#include "inter.IDraw.h"

namespace Shapes
{

class Base;
class Ray;
class BBox;
class Sphere;
class AABB;
class AABBMM;
class Valec;
class Capsule;
class Triangle;
class Frustum;
class MultyShape;
class Plane;

#define SHAPE_INTERFACE  \
protected: \
	virtual void Drawer(); \
public: \
	virtual void DrawInicialize(); \
	virtual void ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride); \
	virtual void Transform(float Scale, vec4* Rotation, vec4* Translation); \
	virtual int Test(AABB *b); \
	virtual int Test(AABBMM *b); \
	virtual int Test(Ray *b, float *distance); \
	virtual int Test(BBox *b); \
	virtual int Test(Sphere *b); \
	virtual int Test(vec *bod); \
	virtual int Test(Triangle *b); \
	virtual int Test(Capsule *b); \
	virtual int Test(Valec *b); \
	virtual int Test(Frustum *b); \
	virtual int Test(Plane *b); \
	virtual vec getAbsMin(); \
	virtual vec getAbsMax(); \
	friend class Base; \
	friend class Ray; \
	friend class AABB; \
	friend class BBox; \
	friend class Sphere; \
	friend class Valec; \
	friend class Capsule; \
	friend class MultyShape; \
	friend class Frustum; \
	friend class Triangle; \
	friend class AABBMM; \

 
class Base : 
	public IDraw
{
protected:
	Base() {}; // abstract

public:
	// Testuj koliziu
	virtual int Test(AABB *b)= 0; 
	virtual int Test(AABBMM *b)= 0; 
	virtual int Test(Ray *b, float *distance) = 0; 
	virtual int Test(BBox *b) = 0; 
	virtual int Test(Sphere *b) = 0; 
	virtual int Test(vec *bod) = 0; 
	virtual int Test(Triangle *b) = 0;
	virtual int Test(Valec *b) = 0;
	virtual int Test(Frustum *b) = 0;
	virtual int Test(Capsule *b) = 0;
	virtual int Test(Plane *b) = 0;
	virtual int Test(MultyShape *b);

	virtual void ComputeFromPoints(unsigned int Count, const vec* pPoints, unsigned int Stride) = 0;
	virtual void Transform(float Scale, vec4* Rotation, vec4* Translation) = 0;

	virtual AABBMM* toAABBMM();
	virtual BBox* toBBox();
	virtual Sphere* toSphere();
	int Test(Ray *b); 
	int Test(Ray *b, vec* HitVectorFromRay); 
	//virtual void getTransformMatrix() = 0;		

	// Ziskaj informacie
	virtual vec getAbsMin() = 0;
	virtual vec getAbsMax() = 0;
	virtual vec getOrigin() = 0;
	virtual vec getPoint(int i) { return getOrigin(); }
	virtual void setOrigin(vec &a) = 0;
	virtual void DrawSustavu(const float size = 1.0);
	//void NajdiBod(vec &o);
};

}
#endif