#ifndef H_Math
#define H_Math
#include <math.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include "BaseMath.h"
#include "class.Vec.h"

// Jednoducha matika ;)
namespace Math 
{
	//vec StredAB(vec * A, vec * B);
	inline double RadianNaUhol(float radianov);
	double UholNaRadiany(float uhol);
	inline double RadianNaUhol(int radianov);
	inline bool real_cmp(double r1, double r2);
	inline vec MakeNormal( const vec &a, const vec &b, const vec &c);

	vec ReflectedRay( const vec &in, const vec &n);
	float PlaneDistance( const vec &normal, const vec &point);
	float PlanePointDelta( const vec &normal, float distance, const vec &point);
	float PlanePointDistance( const vec &normal, float distance, const vec &point);
	vec ClosestPointOnLine( const vec &a, const vec &b, const vec &point);
	int SpherePlaneCollision( const vec &center, float radius, const vec &normal, float distance, float *distanceCenterToPlane );
	vec Interpolate( vec a, const vec &b, float t);
	int PointInsidePolygon( const vec &point, const vec &a, const vec &b, const vec &c);
	int PointInsidePolygon( const vec &point, const vec v[], unsigned int count);
	int EdgeSphereCollision( const vec &center, float radius, const vec &a, const vec &b, const vec &c);
	int EdgeSphereCollision( const vec &center, float radius, vec v[], unsigned int count);
	int SpherePolygonCollision(const vec &center, float radius, vec v[], unsigned int count, const vec &normal);
	int SpherePolygonCollision(const vec &center, float radius, const vec &a, const vec &b, const vec &c, const vec &normal);
	int SpherePolygonCollisionRadius05(const vec &center, float radius, const vec &a, const vec &b, const vec &c, const vec &normal);
	
	int LinePlaneCollision( const vec &a, const vec &b, const vec &normal, float distance);
	vec LinePlaneIntersectionDir( const vec &a, const vec &ab, const vec &normal, float distance);
	float LinePlaneIntersectionDirParameter( const vec &a, const vec &ab, const vec &normal, float distance);
	vec LinePlaneIntersection( const vec &a, const vec &b, const vec &normal, float distance);
	int LinePolygonCollision( const vec &a, const vec &b, vec v[], unsigned int count, const vec &normal);
	int LineSphereIntersectionDir( const vec &p1, const vec &p12, const vec &center, float radius, double *t1, double *t2 );
	int LineSphereIntersection( const vec &p1, const vec &p2, const vec &center, float radius, double *t1, double *t2 );

	vec RefractedRayin(const vec &in, const vec &n, float nri);
	vec RefractedRayi( vec &in, const vec &n, float nri);
	vec RefractedRay( vec &in, const vec &n, float nr);
	vec RefractedRayic( vec &in, const vec &n, float nri);

	float FresnelF0( float nr);
	float Fresnelin( const vec &in, const vec &n, float nri, float f0);
	float Fresneli( const vec &in, const vec &n, float nri, float f0);
	float Fresnel( const vec &in, const vec &n, float nr, float f0);
	float Fresnelin_1( const vec &in, const vec &n, float nri, float f0);
	float Fresnelin_3( const vec &in, const vec &n, float nri, float f0);
	float Fresnel2in( const vec &in, const vec &n, float nri);
	float Fresnel2( const vec &in, const vec &n, float nr);
	
	vec Bspline( const vec &p0, const vec &p1, const vec &p2, const vec &p3, float t);
	vec BsplineDeriv( const vec &p0, const vec &p1, const vec &p2, const vec &p3, float t);
	vec HermiteCurve( const vec &p0, const vec &p1, const vec &t0, const vec &t1, float t);
	vec HermiteCurveDeriv( const vec &p0, const vec &p1, const vec &t0, const vec &t1, float t);
	double AngleBetweenVectors( const vec &a, const vec &b);
	
	inline char isInfPlus( const float &x);
	inline char isInfMinus( const float &x);

	inline float DOT3( const vec &a, const vec &b );
	inline vec CROSS ( const vec &a, const vec &b );
	inline vec Normalize( const vec &a);
	inline float Length( const vec &a );
	inline float Length2( const vec &a );
	inline float Distance ( const vec &a, const vec &b);
	inline float Distance2 ( const vec &a, const vec &b);

	float CROSS(const vec2 &O, const vec2 &A, const vec2 &B);
	std::vector<vec2> ConvexHull(std::vector<vec2> P);
}

#endif
