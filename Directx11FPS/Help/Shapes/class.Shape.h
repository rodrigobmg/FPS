#ifndef H_Shapes
#define H_Shapes
#include "../Math/xnacollision.h"

namespace Shape
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

#define Vertex vec4
#define SHAPE_INTERFACE  \
	virtual void Draw(vec4 color); \
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

 
class Base {
protected:
	vec4 color;
	
	void DrawVertex(Vertex *list, int count);
	void DrawLine(vec &a, vec &b, vec4 x);
	Base() {}; // abstract

public:
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

	virtual vec getAbsMin() = 0;
	virtual vec getAbsMax() = 0;
	virtual vec getOrigin() = 0;
	virtual vec getPoint(int i) { return getOrigin(); }
	virtual vec4 getColor() { return color; }
	virtual void setOrigin(vec &a) = 0;
	virtual void setColor(vec4 x) { color = x; }

	virtual void Draw() { Draw(color); }
	virtual void Draw(vec4 color) = 0;
	virtual void DrawSustavu(float size = 1.0);
	virtual void NajdiBod(vec &o);
	//virtual ~Base();
	//virtual vec getPovrch(vec *a); // najde bod na povrchu objektu ktory je medzi origin a A -  prienik s priamkou
};

}
#endif