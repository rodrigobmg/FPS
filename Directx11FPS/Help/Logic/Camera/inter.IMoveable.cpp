#include "inter.IMoveable.h"
using namespace Camera;

IMoveable::IMoveable() {
	m_pohybspeed.set(30.0f);
	m_sipkyspeed.set(1.0f);
	m_mousespeed.set(0.3f);
		
	assert(gInput);
	m_input = gInput;
	m_moved = true;
}
void IMoveable::InvertMouse() {
	m_mousespeed.y *= -1.0;
}
void IMoveable::ProccessKeys(float delta) 
{		
	float radians;
	m_moved = false;
	POINT mouse = m_input->getRozdiel();
	delta = 1.0f;
	

	// Pohyb mysky
	if( mouse.x != 0) {
		GetRotation().y -= delta * (float) mouse.x * m_mousespeed.x;
		m_moved = true;
		if(GetRotation().y > 360.0f) {
			GetRotation().y -= 360.0f;
		}
	}
	if(mouse.y != 0) {	
		GetRotation().x -= delta * (float) mouse.y * m_mousespeed.y;
		m_moved = true;
		if(GetRotation().x > 90.0f) {
			GetRotation().x = 90.0f;
		}
	}
		
	// Pohyby dopredu / dozadu
	if( m_input->getKeydown('W')) {
		m_moved = true;
		radians = GetRotation().y * 0.0174532925f;
		GetPosition().x += sinf(radians) * delta * m_pohybspeed.x;
		GetPosition().z += cosf(radians) * delta * m_pohybspeed.x;
	}
	if( m_input->getKeydown('S')) {
		m_moved = true;
		radians = GetRotation().y * 0.0174532925f;
		GetPosition().x -= sinf(radians) * delta * m_pohybspeed.x;
		GetPosition().z -= cosf(radians) * delta * m_pohybspeed.x;
	}
	

	// Bocne pohyby ....
	if( m_input->getKeydown('A')) {	
		m_moved = true;
		radians = GetRotation().y * 0.0174532925f + ((float)PI)/2.0f;
		GetPosition().x -= sinf(radians) * delta * m_pohybspeed.z;
		GetPosition().z -= cosf(radians) * delta * m_pohybspeed.z;
	}	
	if( m_input->getKeydown('D')) {
		m_moved = true;
		radians = GetRotation().y * 0.0174532925f + ((float)PI)/2.0f;
		GetPosition().x += sinf(radians) * delta * m_pohybspeed.z;
		GetPosition().z += cosf(radians) * delta * m_pohybspeed.z;
	}
	
	//Vyskove
	if( m_input->getKeydown('Q')) {
		m_moved = true;
		GetPosition().y += delta * m_pohybspeed.y;
	}
	if( m_input->getKeydown('E')) {
		m_moved = true;
		GetPosition().y -= delta * m_pohybspeed.y;
	}

	// Pohyby so sipkamy
	if( m_input->getKeydown(VK_RIGHT)) {
		GetRotation().y += delta * m_sipkyspeed.y;
		m_moved = true;

		if(GetRotation().y > 360.0f) {
			GetRotation().y -= 360.0f;
		}
	}
	if( m_input->getKeydown(VK_LEFT)) {
		GetRotation().y -= delta * m_sipkyspeed.y;
		m_moved = true;

		if(GetRotation().y < 0.0f) {
			GetRotation().y += 360.0f;
		}
	}
	if( m_input->getKeydown(VK_DOWN)) {
		GetRotation().x += delta * m_sipkyspeed.x;
		m_moved = true;
		if(GetRotation().x < -90.0f) {
			GetRotation().x = -90.0f;
		}
	}
	if( m_input->getKeydown(VK_UP)) {
		GetRotation().x -= delta * m_sipkyspeed.x;
		m_moved = true;
		if(GetRotation().x > 90.0f) {
			GetRotation().x = 90.0f;
		}
	}
}
