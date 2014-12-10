#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_
#include <d3dx10math.h>

/*
	Lighting calculations can only be done if each face in your geometry has a normal,
	A normal is a perpendicular vector pointing away from the front side of a face;
*/
class Light
{
public:
	void SetDiffuseColor(float red, float green, float blue, float alpha) { m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha); }
	void SetDirection(float x, float y, float z) { m_direction = D3DXVECTOR3(x, y, z); }
	D3DXVECTOR4 GetDiffuseColor() { return m_diffuseColor; }
	D3DXVECTOR3 GetDirection() { return m_direction; }

private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};

#endif