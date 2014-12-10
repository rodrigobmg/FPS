#include "../Math.h"

namespace Math 
{
	vec StredAB(vec * A, vec * B)
	{
		vec S;
		S.x = (A->x + B->x) / 2.0f;
		S.y = (A->y + B->y) / 2.0f;
		S.z = (A->z + B->z) / 2.0f;
		return S;
	}
	double RadianNaUhol(float radianov) {
		return radianov * 180.0f / PI;
	}
	double UholNaRadiany(float uhol) {
		return uhol *  PI / 180.0f;
	}
	bool real_cmp(double r1, double r2) { return (ABS(r1 - r2) < EPSILON);  }
	

	vec MakeNormal( const vec &a, const vec &b, const vec &c) {
		return Normalize(CROSS( b - a, c - a));
	}
	vec ReflectedRay( const vec &in, const vec &n){ // in from eye to vertex
		return (in - 2.f*n*DOT3( in, n));
	}
	float PlaneDistance( const vec &normal, const vec &point) {
		return -DOT3( normal, point);	//	D = - Ax + By + Cz;   plane equation is Ax + By + Cz + D = 0
	}
	float PlanePointDelta( const vec &normal, float distance, const vec &point) {
		// if point is on normal side, return positive value, else return negative value
		return DOT3( normal, point) + distance;			// d = Ax + By + Cz + D
	}
	float PlanePointDistance( const vec &normal, float distance, const vec &point)
	{	
		// return always positive value
		return ABS(DOT3( normal, point) + distance);			// distance = ABS(Ax + By + Cz + D)
	}

	// najblisi bod na usecke a,b k bodu point
	vec ClosestPointOnLine( const vec &a, const vec &b, const vec &point)
	{
		vec ap = point - a;			// vector from a to point
		vec ab = b-a;				// vector from a to b
		float d = ab.Length();		// = Distance( a, b);  // length Line
		ab.Normalize();				// normalized direction vector from a to b
		float t = DOT3( ab, ap);	// ab is unit vector, t is distance from a to point projected on line ab
		if( t<= 0) return a;		// point projected on line ab is out of line, closest point on line is a
		if( t>= d) return b;		// point projected on line ab is out of line, closest point on line is b
		return a+t*ab;				// point away from a on t length in direction ab
	}
	int SpherePlaneCollision( const vec &center, float radius, const vec &normal, float distance, float *distanceCenterToPlane )
	{
		*distanceCenterToPlane = DOT3( normal, center) + distance;			// d = Ax + By + Cz + D
		if( fabs(*distanceCenterToPlane) < radius) return 1;	// sphere intersected plane
	//	if( *distanceCenterToPlane<= -radius) return 2;			// sphere behind plane
	//	else return 0;											// sphere is front plane
		return 0;												// sphere is front or behind plane
	}
	// return, t=0 a, t=1 b
	vec Interpolate( vec a, const vec &b, float t)
	{
		if(a==b)return a;
		float angle = (float)AngleBetweenVectors( a, b);
		a.ArbitraryRotate( angle*t/PI180, CROSS( a, b));
		return a;
	}

	int PointInsidePolygon( const vec &point, const vec &a, const vec &b, const vec &c)
	{
		vec pa = a-point;
		vec pb = b-point;
		vec pc = c-point;
		double angle  = AngleBetweenVectors( pa, pb);
		angle += AngleBetweenVectors( pb, pc);
		angle += AngleBetweenVectors( pc, pa);

		const double MATCH_FACTOR = 0.99;			// Used to cover up the error in floating point
		if(angle >= (MATCH_FACTOR * (2.0 * PI)) )	// If the angle is greater than 2 PI, (360 degrees)
			return 1;							// The point is inside of the polygon
		return 0;								// If you get here, it obviously wasn't inside the polygon, so Return FALSE
	}

	int PointInsidePolygon( const vec &point, const vec v[], unsigned int count)
	{
		double angle = 0.0;
		
		for (int i = 0; (unsigned int)i<count; i++)				// Go in a circle to each vertex and get the angle between
			angle += AngleBetweenVectors( v[i] - point, v[(i+1)%count] - point);
												
		const double MATCH_FACTOR = 0.99;			// Used to cover up the error in floating point
		if(angle >= (MATCH_FACTOR * (2.0 * PI)) )	// If the angle is greater than 2 PI, (360 degrees)
			return 1;							// The point is inside of the polygon
		return 0;								// If you get here, it obviously wasn't inside the polygon, so Return FALSE
	}

	int EdgeSphereCollision( const vec &center, float radius, const vec &a, const vec &b, const vec &c)
	{
		if( Distance(center, ClosestPointOnLine( a, b, center)) < radius) return 1;
		if( Distance(center, ClosestPointOnLine( b, c, center)) < radius) return 1;
		if( Distance(center, ClosestPointOnLine( c, a, center)) < radius) return 1;
		return 0;
	}

	int EdgeSphereCollision( const vec &center, float radius, vec v[], unsigned int count)
	{
		for(int i = 0; (unsigned int)i<count; i++)
			if( Distance(center, ClosestPointOnLine(v[i], v[(i+1)%count], center)) < radius) return 1;
		return 0;
	}

	int SpherePolygonCollision(const vec &center, float radius, vec v[], unsigned int count, const vec &normal)
	{
		float distance = PlaneDistance( normal, v[0] );
		float distanceCenterToPlane;

		if(SpherePlaneCollision( center, radius, normal, distance, &distanceCenterToPlane) == 1) 
		{
			vec centerProjOnPlane = center - distanceCenterToPlane*normal;			// center project on plane

			if( PointInsidePolygon( centerProjOnPlane, v, count ) ||
				EdgeSphereCollision( center, radius, v, count ) )
			{
				return 1;
			}
		}
		return 0;
	}

	int SpherePolygonCollision(const vec &center, float radius, const vec &a, const vec &b, const vec &c, const vec &normal)
	{
		float distance = PlaneDistance( normal, a );
		float distanceCenterToPlane;

		if(SpherePlaneCollision( center, radius, normal, distance, &distanceCenterToPlane) == 1) 
		{
			vec centerProjOnPlane = center - distanceCenterToPlane*normal;			// center project on plane

			if( PointInsidePolygon( centerProjOnPlane, a, b, c )  ||
				EdgeSphereCollision( center, radius, a, b, c ) )
			{
				return 1;
			}
		}
		return 0;
	}

	int SpherePolygonCollisionRadius05(const vec &center, float radius, const vec &a, const vec &b, const vec &c, const vec &normal)
	{
		float distance = PlaneDistance( normal, a );
		float distanceCenterToPlane;

		if(SpherePlaneCollision( center, radius, normal, distance, &distanceCenterToPlane) == 1) 
		{
			vec centerProjOnPlane = center - distanceCenterToPlane*normal;			// center project on plane

			if( PointInsidePolygon( centerProjOnPlane, a, b, c )  ||
				EdgeSphereCollision( center, 0.5f*radius, a, b, c ) )
			{
				return 1;
			}
		}
		return 0;
	}

	int LinePlaneCollision( const vec &a, const vec &b, const vec &normal, float distance)
	{
		float distance_a_plane = DOT3( normal, a) + distance;
		float distance_b_plane = DOT3( normal, b) + distance;
		if( distance_a_plane*distance_b_plane < 0) return 1;	// must have differently sign
		return 0;
	}

	vec LinePlaneIntersectionDir( const vec &a, const vec &ab, const vec &normal, float distance)
	{
		vec ab_norm = Normalize( ab);
		float distance_plane_a = DOT3( normal, a) + distance;
		float divisor = DOT3( normal, ab_norm);
		if(divisor==0.f) return a;				// line lie on plane
		float t = distance_plane_a/divisor;
		return a-t*ab_norm;
	}

	float LinePlaneIntersectionDirParameter( const vec &a, const vec &ab, const vec &normal, float distance)
	{
		vec ab_norm = Normalize( ab);
		float distance_plane_a = DOT3( normal, a) + distance;
		float divisor = DOT3( normal, ab_norm);
		if(divisor==0.f) return 0.f;				// line lie on plane
		float t = distance_plane_a/divisor;
		return -t/ab.Length();						// for point a t=0, for a+ab t=1.0f 
	}

	vec LinePlaneIntersection( const vec &a, const vec &b, const vec &normal, float distance)
	{
		return LinePlaneIntersectionDir( a, b-a, normal, distance );
	}

	int LinePolygonCollision( const vec &a, const vec &b, vec v[], unsigned int count, const vec &normal)
	{
		float distance = PlaneDistance( normal, v[0]);
		if( !LinePlaneCollision( a, b, normal, distance) ) return 0;

		vec Intersection = LinePlaneIntersection( a, b, normal, distance);
		if( PointInsidePolygon( Intersection, v, count) ) return 1;
		return 0;
	}

	int LineSphereIntersectionDir( const vec &p1, const vec &p12, const vec &center, float radius, double *t1, double *t2 )
	{
	//	vec	p12 = p2-p1;
		double a = DOT3(p12, p12);		// a is always positive
		double b = 2.0*DOT3( p12, p1 - center);
		double c = DOT3( center, center) + DOT3( p1, p1) - 2.0*DOT3( center, p1) - radius*radius;
		double diskriminant = b*b - 4.0*a*c;

		if(diskriminant<0 || a==0){	*t1 = 0;*t2 = 0;return 0;}
		if(diskriminant==0)
		{
			*t1 = ( -b/(2.0*a) );
			*t2 = *t1;
			return 1;
		}
		double s_diskriminant = sqrt(diskriminant);
		*t1 = ( (-b + s_diskriminant)/(2.0*a) );
		*t2 = ( (-b - s_diskriminant)/(2.0*a) );
		// because a is always positive, t1 > t2
		return 2;
	}

	int LineSphereIntersection( const vec &p1, const vec &p2, const vec &center, float radius, double *t1, double *t2 )
	{
		return LineSphereIntersectionDir( p1, p2-p1, center, radius, t1, t2);
	}
	/*  angle1
	   in\  A n-nomala
		  \ |     n1 - absolute refraction index of material 1
		   V|                        sin(angle1)   v1   c/n1   n2        1.0
	   -----+------  nr = n2/n1      ----------- = -- = ---- = -- = nr = ---
			 \                       sin(angle2)   v2   c/n2   n1        nri
			 |    n2 - absolute refraction index of material 2
			 \                       n1*sin(angle1) = n2*sin(angle2)
	   angle2 | T - transmission vector
	in - vector from eye to vertex, must be normalized
	n - normal vector
	nr - relative refraction index, water nr = 1.33
	nri - nri = 1.f/nr
	  T = nri*in - nri*(in*n)*n - sqrt( 1.0-nri^2*(1.0-(in,n)^2) )*n   */
	// *i - call with nri = 1/relative refraction index
	// *n - vector in is Normalized
	vec RefractedRayin(const vec &in, const vec &n, float nri)	// in from eye to vertex
	{
	//	float nri=1.f/nr;
		float dot_in_n = DOT3(in,n);
		float a = (float)sqrt( 1.f-nri*nri*(1.f-dot_in_n*dot_in_n) );
		return nri*in - (a+nri*dot_in_n)*n;
	}
	vec RefractedRayi( vec &in, const vec &n, float nri)		// normalize in
	{	return RefractedRayin( Normalize(in), n, nri);}
	vec RefractedRay( vec &in, const vec &n, float nr)
	{	return RefractedRayin( Normalize(in), n, 1.f/nr);}

	// work also if DOT3( in, n) > 0.f
	vec RefractedRayic( vec &in, const vec &n, float nri)	// in from eye to vertex
	{
		if( DOT3( in, n) > 0.f)
			return RefractedRayi( in, -n, 1.f/nri);
		else 
			return RefractedRayi( in, n, nri);
	}

	float FresnelF0( float nr)
	{
		float nri=1.f/nr;
		float f0,f0_;
		f0 = 1-nri;		//      (1-nri)^2
		f0 *= f0;		// f0 = ----------
		f0_ = 1+nri;	//      (1+nri)^2
		f0_ *= f0_;
		f0 /= f0_;
		return f0;
	}
	// Fresnel
	float Fresnelin( const vec &in, const vec &n, float nri, float f0)
	{
		float dot = DOT3( n, -in);
		float dot2 = dot*dot;
		dot *= dot2*dot2;		// dot = (n*v)^5
		return f0+(1.f-f0)*dot;
	}
	float Fresneli( const vec &in, const vec &n, float nri, float f0)
	{	return Fresnelin( Normalize(in), n, nri, f0);}
	float Fresnel( const vec &in, const vec &n, float nr, float f0)
	{	return Fresnelin( Normalize(in), n, 1.f/nr, f0);}

	float Fresnelin_1( const vec &in, const vec &n, float nri, float f0)
	{
		float dot = DOT3( n, -in);	// dot = (n*v)^1
		return f0+(1.f-f0)*dot;
	}
	float Fresnelin_3( const vec &in, const vec &n, float nri, float f0)
	{
		float dot = DOT3( n, -in);
		dot *= dot*dot;				// dot = (n*v)^3
		return f0+(1.f-f0)*dot;
	}

	char isInfPlus( const float &x)
	{
		float a;
		*(unsigned int*)&a = (unsigned int)0x7f800000;
		return ( x==a);
	//	return (*(unsigned int*) &x==0x7f800000);
	}

	float Fresnel2in( const vec &in, const vec &n, float nri)
	{
		float k,g;
		
		k = DOT3( in, n);
		g = (float)sqrt(nri*nri+k*k-1);

		float gmk = g-k;
		float gpk = g+k;

		float a = k*gpk-1;
		float b = k*gmk+1;
		return (gmk*gmk/(2*gpk*gpk)) *(1+(a*a)/(b*b));
	}
	float Fresnel2( const vec &in, const vec &n, float nr) {	return Fresnel2in( Normalize(in), n, 1.f/nr);}
	char isInfMinus( const float &x)
	{
		float a;
		*(unsigned int*)&a = (unsigned int)0xff800000;
		return ( x==a);
	//	return (*(unsigned int*) &x==0xff800000);
	}
	vec Bspline( const vec &p0, const vec &p1, const vec &p2, const vec &p3, float t)
	{
		float t2=t*t;		// t^2
		float c3=t2*t;		// t^3
		float c0,c1,c2;

		c0 = 1.f - t;	c0 *= c0*c0;		// c0 = (1-t)^3
		c1 = 3.f*c3 - 6.f*t2 + 4;			// c1 = 3*t^3 - 6*t^2 + 4
		c2 =-3.f*c3 + 3.f*t2 + 3.f*t + 1.f;	// c2 =-3*t^3 + 3*t^2 + 3*t + 1
	//	c3 = t*t*t;
	//	vec v = c0*p0 + c1*p1 + c2*p2 + c3*p3;
	//	v *= 0.166666666666666666666666666666667f;	// 1/6
		return 0.166666666666666666666666666666667f*(c0*p0 + c1*p1 + c2*p2 + c3*p3);
	}
	float DOT3( const vec &a, const vec &b ) {
		return (a.x*b.x + a.y*b.y + a.z*b.z);
	}
	vec CROSS ( const vec &a, const vec &b ) {
		return vec( a.y*b.z - a.z*b.y,			//  i   j   k
						a.z*b.x - a.x*b.z,			// a.x a.y a.z
						a.x*b.y - a.y*b.x );		// b.x b.y b.z
	}
	inline float CROSS(const vec2 &O, const vec2 &A, const vec2 &B) {
		return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
	}
	vec BsplineDeriv( const vec &p0, const vec &p1, const vec &p2, const vec &p3, float t)
	{
		float t2=t*t;		// t^2
		float c0,c1,c2,c3;
	//	(1-t)^3 = (1-t)*(1-t-t+t^2) = (1-t)*(1-2*t+t^2) = (1-2*t+t^2 -t +2*t^2 - t^3) = (1-3*t+3*t^2-t^3)
		c0 =-3.f*t2 + 6.f*t - 3;			// c0 =-3*t^2 + 6*t - 3
		c1 = 9.f*t2 -12.f*t;				// c1 = 9*t^2 -12*t
		c2 =-9.f*t2 + 6.f*t + 3;			// c2 =-9*t^2 + 6*t + 3
		c3 = 3.f*t*t;						// c3 = 3*t^2
		return 0.166666666666666666666666666666667f*(c0*p0 + c1*p1 + c2*p2 + c3*p3);
	}

	vec HermiteCurve( const vec &p0, const vec &p1, const vec &t0, const vec &t1, float t)
	{
		float t2 = t*t;
		float t3 = t2*t;
		float h1 =  2*t3 - 3*t2 + 1;
		float h2 = -2*t3 + 3*t2;
		float h3 =    t3 - 2*t2 + t;
		float h4 =    t3 -   t2;
		return h1*p0 + h2*p1 + h3*t0 + h4*t1;
	}

	vec HermiteCurveDeriv( const vec &p0, const vec &p1, const vec &t0, const vec &t1, float t)
	{
		float t2 = t*t;
		float h1 =  6*t2 - 6*t;
		float h2 = -6*t2 + 6*t;
		float h3 =  3*t2 - 4*t + 1;
		float h4 =  3*t2 - 2*t;
		return h1*p0 + h2*p1 + h3*t0 + h4*t1;
	}
	double AngleBetweenVectors( const vec &a, const vec &b)
	{							
		double angle = acos( DOT3( a, b) / ( a.Length()*b.Length() ) );
	//	if(_isnan(angle))return 0;		// Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinate
		if( isInfPlus( (float)angle)||isInfMinus( (float)angle) )return 0;
		return( angle );
	}
	vec Normalize( const vec &a){	return a/a.Length();}
	float Length( const vec &a ){	return a.Length();}
	float Length2( const vec &a ){	return a.Length2();}
	float Distance ( const vec &a, const vec &b){	return Length( a-b );}
	float Distance2 ( const vec &a, const vec &b){	return Length2( a-b );}


	
	// Implementation of Andrew's monotone chain 2D convex hull algorithm.
	// Asymptotic complexity: O(n log n).
	// Practical performance: 0.5-1.0 seconds for n=1000000 on a 1GHz machine.
	// http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
	// Returns a list of points on the convex hull in counter-clockwise order.
	// Note: the last point in the returned list is the same as the first one.
	std::vector<vec2> ConvexHull(std::vector<vec2> P)
	{
		int n = P.size(), k = 0, i, t;
		std::vector<vec2> H(2*n);

		// Sort points lexicographically
		std::sort(P.begin(), P.end());

		// Build lower hull
		for(i = 0; i < n; i++) {
			while (k >= 2 && CROSS(H[k-2], H[k-1], P[i]) <= 0) k--;
			H[k++] = P[i];
		}

		// Build upper hull
		for(i = n-2, t = k+1; i >= 0; i--) {
			while (k >= t && CROSS(H[k-2], H[k-1], P[i]) <= 0) k--;
			H[k++] = P[i];
		}

		H.resize(k);
		return H;
	}
}

