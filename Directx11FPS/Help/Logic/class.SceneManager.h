#pragma once
#include "../App/class.Game.h"
#include "../Base/Structures/class.DynArray.h"
#include "class.Scene.h"


class SceneManager : 
	public Module, 
	protected DynArray<Scene*> 
{	
protected:
	virtual void Frame(SFrame& s);
	void Init();
	void Precache();
	void End();

public :
	SceneManager();
	void Clear();
	~SceneManager();
};


// *Scene graph might give you more flexibility, which might be useful later. 
// For example you could have entities that would be disabled and would not need to process the update event. 
// They might get activated in certain time and then become active 
// (eg. when player triggers an event like entering certain area or pushing a button.) 
// Until that, they would be hidden in scene graph hierarchy and not bothered by the program.



inline SceneManager::SceneManager() {
	// Dalej
	Precache();
	Init();
}
inline void SceneManager::Precache() {
	for(UINT i=0; i < GetSize(); i++) {
		if(!GetAt(i)->Precache()) {
			throw new std::exception("Scena nenacachovala vsetko.");
			break;
		}
	}
}
inline void SceneManager::Init() {
	for(UINT i=0; i < GetSize(); i++) {
		GetAt(i)->Init();
	}
}
inline void SceneManager::Frame(SFrame& s) {
	double d;
	d = s.GetDeltaTime();
	for(UINT i=0; i < GetSize(); i++) {
		GetAt(i)->Frame(d);
	}
}
inline void SceneManager::End() {
	for(UINT i=0; i < GetSize(); i++) {
		GetAt(i)->End();
	}
}
inline void SceneManager::Clear() {
	RemoveAll();
}
inline SceneManager::~SceneManager() {
	End();
	Clear();
}