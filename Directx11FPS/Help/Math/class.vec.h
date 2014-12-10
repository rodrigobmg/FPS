#ifndef _VECTOR_H
#define _VECTOR_H
#pragma once

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#define VEC_INTERFACE(NAME) \
	NAME() {} \
	NAME( const float a ); \
	NAME( const vec &a); \
	NAME( const vec2 &a); \
	NAME( const vec4 &a); \
	void set(const float a); \
	NAME operator+() const; \
	NAME operator-() const;	\
	void operator+= ( const NAME &a); \
	void operator-= ( const NAME &a); \
	void operator*= ( const NAME &a); \
	void operator/= ( const NAME &a); \
	void operator+= ( const float f); \
	void operator-= ( const float f); \
	void operator*= ( const float f); \
	void operator/= ( const float f); \
	friend NAME operator+ ( const NAME &a, const NAME &b ); \
	friend NAME operator- ( const NAME &a, const NAME &b ); \
	friend NAME operator* ( const NAME &a, const NAME &b ); \
	friend NAME operator/ ( const NAME &a, const NAME &b ); \
	friend NAME operator+ ( const float f, const NAME &a ); \
	friend NAME operator- ( const float f, const NAME &a ); \
	friend NAME operator* ( const float f, const NAME &a ); \
	friend NAME operator/ ( const float f, const NAME &a ); \
	friend NAME operator+ ( const NAME &a, const float f ); \
	friend NAME operator- ( const NAME &a, const float f ); \
	friend NAME operator* ( const NAME &a, const float f ); \
	friend NAME operator/ ( const NAME &a, const float f ); \
	friend int operator< ( const NAME &a, const NAME &b ); \
	friend int operator<=( const NAME &a, const NAME &b ); \
	friend int operator> ( const NAME &a, const NAME &b ); \
	friend int operator>=( const NAME &a, const NAME &b ); \
	friend int operator==( const NAME &a, const NAME &b ); \
	friend int operator==( const NAME &a, const float f ); \
	friend int operator!=( const NAME &a, const NAME &b ); \
	friend int operator!=( const NAME &a, const float f ); \
	float Length() const; \
	float Length2() const; \
	void Random(float a, float b); \
	void Random(NAME * a, NAME * b); \
	void Normalize(); \
	void Abs();	

class vec;
class vec2;
class vec4;

class vec : public D3DXVECTOR3
{
public:
	VEC_INTERFACE(vec)
	vec( const float a, const float b, const float c);
	void set( const float a, const float b, const float c);

	vec Cross( const vec &a) const;
	void Cross( const vec &a, const vec &b);
	float DOT3( const vec &a) const;
	void ArbitraryRotate(float theta, vec r);

	void RotX( const float angle);
	void RotY( const float angle);
	void RotZ( const float angle);

	operator vec4() const;
	operator D3DXVECTOR3() const;
};


class vec2 : public D3DXVECTOR2  
{
public:
	VEC_INTERFACE(vec2)
	vec2( const float a, const float b);
	void set( const float a, const float b);

	float DOT3( const vec2 &a) const;
	void RotX( const float angle);
	void RotY( const float angle);
};


class vec4 : public D3DXVECTOR4
{
public:
	VEC_INTERFACE(vec4)
	vec4( const float a, const float b, const float c, const float d = 1.f);
	void set( const float a, const float b, const float c, const float d = 1.f);
};

#endif





