#pragma once
#include "../Base/class.Base.h"

class Player : public Entity::Base 
{
private:
	Shape::Frustum m_frustum;
	std::string m_name;
	bool m_connected;
	bool m_alive;

protected:
	// Osoba bola zabyta
	virtual void Kill() {

	}

	// Spawni osobu
	virtual void Spawn() {

	}

	// Zaciatok kola
	virtual void RoundStart() {

	}
	
	// Na konci kola
	virtual void RoundEnd() {

	}
	
	// Pripoji sa alebo uplne vznikne alebo spusta server
	virtual void Connect() {
		
	}

	// Odpoji sa z hry  alebo sa server vypina, cize cast len co sa odpoji zo servera
	virtual void Disconnect() {

	}

	// Objekt sa dostane mimo sveta
	virtual void GoOutside() {

	}

	// Vymaz dalsie casti
	virtual void Destroyed() { 
		
	}

	virtual void PreThink() {}
	virtual void Think(double d) {}
	virtual void PostThink() {}
}
/*
	Pick a few points on the target's body (e.g. head, chest, knees) 
	is if those points lie within the AI's view-cone, 
	perform a ray-cast to determine if they're visible.

	*Kazda osoba ma Frustrum a cez ten testujeme
	*A potom ray-cast
*/