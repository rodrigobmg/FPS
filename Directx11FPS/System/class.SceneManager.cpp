#include "class.Managers.h"

SceneManager::SceneManager() {
	// Dalej
	Precache();
	Init();
}
void SceneManager::Precache() {
	for(UINT i=0; i < GetSize(); i++) {
		if(!GetAt(i)->Precache()) {
			throw new exception("Scena nenacachovala vsetko.");
			break;
		}
	}
}
void SceneManager::Init() {
	for(UINT i=0; i < GetSize(); i++) {
		GetAt(i)->Init();
	}
}
void SceneManager::Frame() {
	double d;
	d = gGame->GetLeftTime();
	for(UINT i=0; i < GetSize(); i++) {
		GetAt(i)->Frame(d);
	}
}
void SceneManager::End() {
	for(UINT i=0; i < GetSize(); i++) {
		GetAt(i)->End();
	}
}
void SceneManager::Clear() {
	RemoveAll();
}
SceneManager::~SceneManager() {
	End();
	Clear();
}