#pragma once
#include "DesignPattern.h"
#include "../App/Base/class.WinDebug.h"
#include "System/class.Time.h"
#include <map>

class Profiler;

class Profiler : public Time 
{
	/* TODO
	-Average, min, max
	*/	

private:
	SINGLETON(Profiler)
	std::map<const char*,double> m_map;

public:
	Profiler() { 
		WinDebug::getInstance();
	}
	void Start(const char* name) {
		m_map[name] = Time::getInstance().GetAbsolute();
	}
	void End(const char* name) {
		double time = Time::getInstance().GetAbsolute() - m_map[name];
		m_map[name] = time;
		printf("%s %.2f %%\n", name, time);
	}

	/*
	*	Trochu iny typ profileru,...
	*/

	class Region {
	private:
		friend class Region;
		DynArray<Region*> m_next;
		Region* m_parent;
		const char* m_name;
		double m_taketime;
		void* m_puser;
		DWORD m_level;

	public:	
		// TODO* dalsie constructoty ako start
		Region() {
			m_parent = NULL;
			m_name = NULL;
			m_taketime = 0.f;
			m_puser = NULL;
			m_level = 0;
		}
		~Region() {
			if(m_puser) delete m_puser;
		}
		inline void Start() {
			Start(typeid(*this).name());
		}
		inline void Start(const char* name) {
			for(DWORD i=0; i < m_next.GetSize(); i++) delete m_next.GetAt(i);
			m_next.RemoveAll();
			m_name = name;
			m_taketime = Profiler::getInstance().GetAbsolute();
		}
		inline void Start(Region* parent) {
			m_parent = parent;
			m_level = parent->getLevel() + 1;
			parent->m_next.Add(this);
			Start();	
		}
		inline void Start(Region* parent, const char* name) {
			m_parent = parent;
			m_level = parent->getLevel() + 1;
			parent->m_next.Add(this);
			Start(name);	
		}


		void PrintStats() { // aj deti vypis
			// Aspon 1 funkcia v classe musi byt virtualna aby to davalo dobry nazov
			PrintStat();
			for(UINT i=0; i < m_next.GetSize(); i++) {
				m_next.GetAt(i)->PrintStats();
			}
		}
		// Len seba
		inline void PrintStat() { PrintTab(); Msg(m_puser); }
		inline void End() { 
			m_taketime = Profiler::getInstance().GetAbsolute() - m_taketime; 
		}
		inline void End(void *pUser) {
			End();
			//if(m_puser) delete m_puser;
			m_puser = pUser;
		}
		
	protected:
		virtual void Msg(void* pUser) { 
			printf("Region %s %.2f\n", getName(), getTime()); 
		}
		inline double getTime() { return m_taketime; }
		inline float getPercent() { if(m_parent == NULL) return -1.f; return (float) ( (100.0 * getTime()) / m_parent->getTime() ); }
		inline DWORD getLevel() { return m_level; }
		inline const char* getName() { return m_name; }
		inline void PrintTab() { for(DWORD i=0; i < getLevel(); i++) putchar('\t'); }
	};
};