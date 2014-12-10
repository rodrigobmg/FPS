#include <math.h>
#include <stdlib.h>
#include "../class.vec.h"
#include "../BaseMath.h"

vec4::vec4( const float a ) {	x = y = z = w = a; }
vec4::vec4( const float a, const float b, const float c, const float d){	x = a; y = b; z = c; w = d;}
vec4::vec4( const vec &in) {	x = in.x; y = in.y; z = in.z; w = 1.f;}
vec4::vec4( const vec2 &a){	x = a.x; y = a.y; z = 0.f; w = 1.f; }
vec4::vec4( const vec4 &a) {	x = a.x; y = a.y; z = a.z; w = a.w;}

void vec4::set( const float a, const float b, const float c, const float d) {	x = a; y = b; z = c; w = d;}
void vec4::set( const float a) {	x = y = z = w = a; }

void vec4::Random(vec4 *a, vec4 *b) {
	x = ((b->x - a->x)*((float)rand()/RAND_MAX))+a->x;
	y = ((b->y - a->y)*((float)rand()/RAND_MAX))+a->y;
	z = ((b->z - a->z)*((float)rand()/RAND_MAX))+a->z;
	w = ((b->w - a->w)*((float)rand()/RAND_MAX))+a->w;
}
void vec4::Random(float a, float b) { 
	x = ((b-a)*((float)rand()/RAND_MAX))+a; 
	y = ((b-a)*((float)rand()/RAND_MAX))+a;
	z = ((b-a)*((float)rand()/RAND_MAX))+a;
	w = ((b-a)*((float)rand()/RAND_MAX))+a;
}
float vec4::Length() const {	
	return (float) sqrt(x*x + y*y + z*z + w*w);
}
float vec4::Length2() const {	
	return (float) x*x + y*y + z*z + w*w;
}
void vec4::Abs() {	
	if(x < 0.f) { x =  - x; }
	if(y < 0.f) { y =  - y; }
	if(z < 0.f) { z =  - z; }
	if(w < 0.f) { w =  - w; }
}
void vec4::Normalize() {
	float length;
	length = (float) sqrt(x*x + y*y + z*z + w*w);
	if (length != 0) {
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	} else {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
}

//vec4::operator const vec4 () { return vec4(x, y, z, w); }
vec4 vec4::operator+() const	{	return *this; }
vec4 vec4::operator-() const	{	return vec4( -x, -y, -z, -w);}

void vec4::operator+= ( const vec4 &a){	x += a.x; y += a.y; z += a.z; w += a.w; }
void vec4::operator-= ( const vec4 &a){	x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
void vec4::operator*= ( const vec4 &a){	x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
void vec4::operator/= ( const vec4 &a){	x /= a.x; y /= a.y; z /= a.z; w /= a.w; }
void vec4::operator+= ( const float f){	x += f; y += f; z += f; w += f; }
void vec4::operator-= ( const float f){	x -= f; y -= f; z -= f; w -= f; }
void vec4::operator*= ( const float f){	x *= f; y *= f; z *= f; w *= f; }
void vec4::operator/= ( const float f){	x /= f; y /= f; z /= f; w /= f; }

vec4 operator+ ( const vec4 &a, const vec4 &b ){return vec4( a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w );}
vec4 operator- ( const vec4 &a, const vec4 &b ){return vec4( a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w );}
vec4 operator* ( const vec4 &a, const vec4 &b ){return vec4( a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w );}
vec4 operator/ ( const vec4 &a, const vec4 &b ){return vec4( a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w );}

vec4 operator+ ( const float f, const vec4 &a ){return vec4( f+a.x, f+a.y, f+a.z, f+a.w );}
vec4 operator- ( const float f, const vec4 &a ){return vec4( f-a.x, f-a.y, f-a.z, f-a.w );}
vec4 operator* ( const float f, const vec4 &a ){return vec4( f*a.x, f*a.y, f*a.z, f*a.w );}
vec4 operator+ ( const vec4 &a, const float f ){return vec4( a.x+f, a.y+f, a.z+f, a.z+f );}
vec4 operator- ( const vec4 &a, const float f ){return vec4( a.x-f, a.y-f, a.z-f, a.z-f );}
vec4 operator* ( const vec4 &a, const float f ){return vec4( a.x*f, a.y*f, a.z*f, a.z*f );}
vec4 operator/ ( const vec4 &a, const float f ){float f_ = 1.0f/f;return vec4( a.x*f_, a.y*f_, a.z*f_, a.w*f_ );}

int operator< ( const vec4 &a, const vec4 &b ){return ((a.x< b.x)&&(a.y< b.y)&&(a.z< b.z)&&(a.w< b.w));}
int operator<=( const vec4 &a, const vec4 &b ){return ((a.x<=b.x)&&(a.y<=b.y)&&(a.z<=b.z)&&(a.w<=b.w));}
int operator> ( const vec4 &a, const vec4 &b ){return ((a.x> b.x)&&(a.y> b.y)&&(a.z> b.z)&&(a.w> b.w));}
int operator>=( const vec4 &a, const vec4 &b ){return ((a.x>=b.x)&&(a.y>=b.y)&&(a.z>=b.z)&&(a.w>=b.w));}
int operator==( const vec4 &a, const vec4 &b ){return ((a.x==b.x)&&(a.y==b.y)&&(a.z==b.z)&&(a.w==b.w));}
int operator==( const vec4 &a, const float f ){return ((a.x==f)&&(a.y==f)&&(a.z==f)&&(a.w==f));}
int operator!=( const vec4 &a, const vec4 &b ){return ((a.x!=b.x)||(a.y!=b.y)||(a.z!=b.z)||(a.w!=b.w));}
int operator!=( const vec4 &a, const float f ){return ((a.x!=f)||(a.y!=f)||(a.z!=f)||(a.w!=f));}
