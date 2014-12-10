#ifndef H_Module
#define H_Module
#include "Structures/class.DynArray.h"
#include "System/class.Timer.h"
#include "../../Global.h"
#include "class.Profiler.h"
#include "inter.IFrame.h"
#include <typeinfo>
#include <exception>

class Module : public IFrame {
private:
	bool m_paused;
	friend class Modules;

public :
	Module() : m_paused(false) { }
	virtual void PauseToggle() { m_paused = !m_paused; }
	virtual ~Module() {} // lebo inac sa vsetko nevymaze ako chceme
};



class Modules : 
	protected DynArray<Module*>,
	public Profiler::Region
{
private:
	virtual void Msg(void* pUser) {
		printf("\nModul %s - %f sec\n", getName(), getTime());
	}
	class ModuleRegion : public Profiler::Region {
	protected:
		virtual void Msg(void* pUser) {
			printf("%d. (%s) %.2f %%\n", *((int*)pUser), getName(), getPercent());
			//delete pUser;
		}
	};

public :
	// class Pause
	// class Remove
	// class Break

	virtual void RunModules(SFrame& func) {
		Module *modul;
		Region *r;
		int* userData;
		Profiler::Region::Start();
		for(UINT i=0; i < GetSize(); i++) {
			modul = GetAt(i);
			if(!modul->m_paused) {
				r = new ModuleRegion;
				r->Start(this, typeid(*modul).name());
				modul->Frame(func);		
				userData = new int;
				*userData = i;
				r->End((void*) userData);
			}
		}
		Profiler::Region::End();
		// userData a Regiony sa nemazu o to sa stara Profiler
	}
	inline void ModulesOff(){
		RemoveAll();
	}
	inline Module* Add(Module* value) {
		DynArray::Add(value);
		return value;
	}
};

#endif