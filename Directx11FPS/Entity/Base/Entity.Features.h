#pragma once
#include <string>

namespace Entity 
{ 
	extern class Base;

	//+ Interface pre objekt mozny znicit
	class IDestroyable {
		float m_hp;
		bool m_takedmg;	

	protected:
		virtual void DMG(Entity::Base *ent, float dmg, UINT typ) {}
		virtual void Kill() {}
	};

	//+ Interface ktory podporuje animacie
	class IAnimateable {
		std::string m_model;
		float m_animspeed;
		int m_animtype;

	protected:
		virtual void PreThink() {}
		virtual void Think(double d) {}
		virtual void PostThink() {}
	};
}