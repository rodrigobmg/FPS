#include <math.h>
#include <stdlib.h>
#include "../class.vec.h"
#include "../BaseMath.h"

vec2::vec2( const float a ) {	x = y = a; }
vec2::vec2( const float a, const float b){	x = a; y = b;}
vec2::vec2( const vec &a) {	x = a.x; y = a.y; }
vec2::vec2( const vec2 &a){	x = a.x; y = a.y; }
vec2::vec2( const vec4 &a){	x = a.x; y = a.y; }

void vec2::set( const float a, const float b) {	x = a; y = b; }
void vec2::set( const float a) {	x = y = a; }

void vec2::Random(vec2 *a, vec2 *b) {
	x = ((b->x - a->x)*((float)rand()/RAND_MAX))+a->x;
	y = ((b->y - a->y)*((float)rand()/RAND_MAX))+a->y;
}
void vec2::Random(float a, float b) { 
	x = ((b-a)*((float)rand()/RAND_MAX))+a; 
	y = ((b-a)*((float)rand()/RAND_MAX))+a;
}
float vec2::DOT3( const vec2 &a) const {
	return (x*a.x + y*a.y);
}
float vec2::Length() const {	
	return (float) sqrt(x*x + y*y);
}
float vec2::Length2() const {	
	return (float) x*x + y*y;
}
void vec2::RotX( const float angle) {
	x = sqrt( x + y )*cos(angle );
}
void vec2::RotY( const float angle) {
	y = sqrt( x + y )*cos(angle );
}
void vec2::Normalize() {
	float length;
	length = (float) sqrt(x*x + y*y);
	if (length != 0) {
		x /= length;
		y /= length;
	} else {
		x = 0;
		y = 0;
	}
}
void vec2::Abs() {	
	if(x < 0.f) { x =  - x; }
	if(y < 0.f) { y =  - y; }
}

//vec2::operator const vec4 () { return vec4(x, y, 0.0f); }
vec2 vec2::operator+() const	{	return *this; }
vec2 vec2::operator-() const	{	return vec2(-x, -y);}

void vec2::operator+= ( const vec2 &a){	x += a.x; y += a.y; }
void vec2::operator-= ( const vec2 &a){	x -= a.x; y -= a.y; }
void vec2::operator*= ( const vec2 &a){	x *= a.x; y *= a.y; }
void vec2::operator/= ( const vec2 &a){	x /= a.x; y /= a.y; }
void vec2::operator+= ( const float f){	x += f; y += f; }
void vec2::operator-= ( const float f){	x -= f; y -= f; }
void vec2::operator*= ( const float f){	x *= f; y *= f; }
void vec2::operator/= ( const float f){	x /= f; y /= f; }

vec2 operator+ ( const vec2 &a, const vec2 &b ){return vec2( a.x+b.x, a.y+b.y );}
vec2 operator- ( const vec2 &a, const vec2 &b ){return vec2( a.x-b.x, a.y-b.y );}
vec2 operator* ( const vec2 &a, const vec2 &b ){return vec2( a.x*b.x, a.y*b.y );}
vec2 operator/ ( const vec2 &a, const vec2 &b ){return vec2( a.x/b.x, a.y/b.y);}

vec2 operator+ ( const float f, const vec2 &a ){return vec2( f+a.x, f+a.y);}
vec2 operator- ( const float f, const vec2 &a ){return vec2( f-a.x, f-a.y);}
vec2 operator* ( const float f, const vec2 &a ){return vec2( f*a.x, f*a.y );}
vec2 operator+ ( const vec2 &a, const float f ){return vec2( a.x+f, a.y+f );}
vec2 operator- ( const vec2 &a, const float f ){return vec2( a.x-f, a.y-f);}
vec2 operator* ( const vec2 &a, const float f ){return vec2( a.x*f, a.y*f);}
vec2 operator/ ( const vec2 &a, const float f ){float f_ = 1.0f/f; return vec2( a.x*f_, a.y*f_);}

int operator< ( const vec2 &a, const vec2 &b ){return ((a.x< b.x)&&(a.y< b.y));}
int operator<=( const vec2 &a, const vec2 &b ){return ((a.x<=b.x)&&(a.y<=b.y));}
int operator> ( const vec2 &a, const vec2 &b ){return ((a.x> b.x)&&(a.y> b.y));}
int operator>=( const vec2 &a, const vec2 &b ){return ((a.x>=b.x)&&(a.y>=b.y));}
int operator==( const vec2 &a, const vec2 &b ){return ((a.x==b.x)&&(a.y==b.y));}
int operator==( const vec2 &a, const float f ){return ((a.x==f)&&(a.y==f));}
int operator!=( const vec2 &a, const vec2 &b ){return ((a.x!=b.x)||(a.y!=b.y));}
int operator!=( const vec2 &a, const float f ){return ((a.x!=f)||(a.y!=f));}
