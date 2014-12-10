#include <math.h>
#include <stdlib.h>
#include "../class.vec.h"
#include "../BaseMath.h"
#include <d3dx9math.h>

vec::vec( const float a ) {	x = y = z = a; }
vec::vec( const float a, const float b, const float c ){	x = a; y = b; z = c;}
vec::vec( const vec &a) {	x = a.x; y = a.y; z = a.z;}
vec::vec( const vec2 &a){	x = a.x; y = a.y; z = 0.f;}
vec::vec( const vec4 &a){	x = a.x; y = a.y; z = a.z;}

void vec::set( const float a, const float b, const float c) {	x = a; y = b; z = c;}
void vec::set( const float a) {	x = y = z = a; }

void vec::Random(vec *a, vec *b) {
	x = ((b->x - a->x)*((float)rand()/RAND_MAX))+a->x;
	y = ((b->y - a->y)*((float)rand()/RAND_MAX))+a->y;
	z = ((b->z - a->z)*((float)rand()/RAND_MAX))+a->z;
}
void vec::Random(float a, float b) { 
	x = ((b-a)*((float)rand()/RAND_MAX))+a; 
	y = ((b-a)*((float)rand()/RAND_MAX))+a;
	z = ((b-a)*((float)rand()/RAND_MAX))+a;
}
void vec::Abs() {	
	if(x < 0.f) { x =  - x; }
	if(y < 0.f) { y =  - y; }
	if(z < 0.f) { z =  - z; }
}
vec vec::Cross( const vec &b) const {
	return vec( y*b.z - z*b.y,
					z*b.x - x*b.z,
					x*b.y - y*b.x );
}
void vec::Cross( const vec &a, const vec &b) {
	x = a.y * b.z - a.z * b.y;
	y = a.z * b.x - a.x * b.z;
	z = a.x * b.y - a.y * b.x;
}
float vec::DOT3( const vec &a) const {
	return (x*a.x + y*a.y +z*a.z);
}
float vec::Length() const
{	
	return (float) sqrt(x*x + y*y + z*z);
}
float vec::Length2() const {	
	return (float) x*x + y*y + z*z;
}
void vec::RotX( const float angle) {
	float s = (float) sin( PI180*angle );
	float c = (float) cos( PI180*angle );
	float temp_Y=y;
	y =  y*c - z*s;
	z =  temp_Y*s + z*c;
}
void vec::RotY( const float angle) {
	float s = (float) sin( PI180*angle );
	float c = (float) cos( PI180*angle );
	float X=x;
	x =  x*c + z*s;
	z = -X*s + z*c;
}
void vec::RotZ( const float angle) {
	float s = (float) sin( PI180*angle );
	float c = (float) cos( PI180*angle );
	float X=x;
	x =  x*c - y*s;
	y =  X*s + y*c;
}
void vec::Normalize() {
	float length;
	length = (float) sqrt(x*x + y*y + z*z);
	if (length != 0) {
		x /= length;
		y /= length;
		z /= length;
	} else {
		x = 0;
		y = 0;
		z = 0;
	}
}

//	Rotate a vector by angle theta around an arbitrary axis r
//	Positive angles are anticlockwise looking down the axis  towards the origin.
void vec::ArbitraryRotate(float theta, vec r)	// Rotate a vector by angle theta around an arbitrary axis r
{
	vec p(x,y,z);
	float costheta,sintheta;

	theta*=PI180;
	r.Normalize();
	costheta = (float)cos(theta);
	sintheta = (float)sin(theta);

	x  = (costheta + (1 - costheta) * r.x * r.x) * p.x;
	x += ((1 - costheta) * r.x * r.y - r.z * sintheta) * p.y;
	x += ((1 - costheta) * r.x * r.z + r.y * sintheta) * p.z;

	y  = ((1 - costheta) * r.x * r.y + r.z * sintheta) * p.x;
	y += (costheta + (1 - costheta) * r.y * r.y) * p.y;
	y += ((1 - costheta) * r.y * r.z - r.x * sintheta) * p.z;

	z  = ((1 - costheta) * r.x * r.z - r.y * sintheta) * p.x;
	z += ((1 - costheta) * r.y * r.z + r.x * sintheta) * p.y;
	z += (costheta + (1 - costheta) * r.z * r.z) * p.z;
}

vec vec::operator+() const	{	return *this;}
vec vec::operator-() const	{	return vec( -x, -y, -z);}

void vec::operator+= ( const vec &a){	x += a.x; y += a.y; z += a.z;}
void vec::operator-= ( const vec &a){	x -= a.x; y -= a.y; z -= a.z;}
void vec::operator*= ( const vec &a){	x *= a.x; y *= a.y; z *= a.z;}
void vec::operator/= ( const vec &a){	x /= a.x; y /= a.y; z /= a.z;}
void vec::operator+= ( const float f){	x += f; y += f; z += f; }
void vec::operator-= ( const float f){	x -= f; y -= f; z -= f; }
void vec::operator*= ( const float f){	x *= f; y *= f; z *= f; }
void vec::operator/= ( const float f){	x /= f; y /= f; z /= f; }

vec operator+ ( const vec &a, const vec &b ){return vec( a.x+b.x, a.y+b.y, a.z+b.z );}
vec operator- ( const vec &a, const vec &b ){return vec( a.x-b.x, a.y-b.y, a.z-b.z );}
vec operator* ( const vec &a, const vec &b ){return vec( a.x*b.x, a.y*b.y, a.z*b.z );}
vec operator/ ( const vec &a, const vec &b ){return vec( a.x/b.x, a.y/b.y, a.z/b.z );}

vec operator+ ( const float f, const vec &a ){return vec( f+a.x, f+a.y, f+a.z );}
vec operator- ( const float f, const vec &a ){return vec( f-a.x, f-a.y, f-a.z );}
vec operator* ( const float f, const vec &a ){return vec( f*a.x, f*a.y, f*a.z );}
vec operator+ ( const vec &a, const float f ){return vec( a.x+f, a.y+f, a.z+f );}
vec operator- ( const vec &a, const float f ){return vec( a.x-f, a.y-f, a.z-f );}
vec operator* ( const vec &a, const float f ){return vec( a.x*f, a.y*f, a.z*f );}
vec operator/ ( const vec &a, const float f ){float f_ = 1.0f/f;return vec( a.x*f_, a.y*f_, a.z*f_ );}

int operator< ( const vec &a, const vec &b ){return ((a.x< b.x)&&(a.y< b.y)&&(a.z< b.z));}
int operator<=( const vec &a, const vec &b ){return ((a.x<=b.x)&&(a.y<=b.y)&&(a.z<=b.z));}
int operator> ( const vec &a, const vec &b ){return ((a.x> b.x)&&(a.y> b.y)&&(a.z> b.z));}
int operator>=( const vec &a, const vec &b ){return ((a.x>=b.x)&&(a.y>=b.y)&&(a.z>=b.z));}
int operator==( const vec &a, const vec &b ){return ((a.x==b.x)&&(a.y==b.y)&&(a.z==b.z));}
int operator==( const vec &a, const float f ){return ((a.x==f)&&(a.y==f)&&(a.z==f));}
int operator!=( const vec &a, const vec &b ){return ((a.x!=b.x)||(a.y!=b.y)||(a.z!=b.z));}
int operator!=( const vec &a, const float f ){return ((a.x!=f)||(a.y!=f)||(a.z!=f));}


vec::operator vec4() const { return vec4(x, y, z, 1.f); }
vec::operator D3DXVECTOR3() const { return D3DXVECTOR3(x, y, z); }
