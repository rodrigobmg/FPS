#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "../../Math/class.vec.h"

namespace Camera 
{

class Base
{
private:
	vec m_position;
	vec m_rotation;
	D3DXMATRIX m_viewMatrix;

public:
	Base() {
		m_position = vec(0.f);
		m_rotation = vec(0.f);
		PrepareMatrix();
	}

	D3DXMATRIX GetViewMatrix() { return m_viewMatrix; }
	inline vec& GetPosition() { return m_position; }
	inline vec& GetRotation() { return m_rotation; }
	inline void SetPosition(vec &a) { m_position = a; }
	inline void SetRotation(vec &a) { m_rotation = a; }

protected:

	void PrepareMatrix() {
		D3DXVECTOR3 up, position, lookAt;
		float yaw, pitch, roll;
		D3DXMATRIX rotationMatrix;

		// Setup the vector that points upwards.
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		// Setup the position of the camera in the world.
		position = m_position;

		// Setup where the camera is looking by default.
		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		pitch = m_rotation.x * 0.0174532925f;
		yaw   = m_rotation.y * 0.0174532925f;
		roll  = m_rotation.z * 0.0174532925f;

		// Create the rotation matrix from the yaw, pitch, and roll values.
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

		// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
		D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
		D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

		// Translate the rotated camera position to the location of the viewer.
		lookAt = position + lookAt;

		// Finally create the view matrix from the three updated vectors.
		D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	}
};


}