#pragma once
#include "Base/class.Base.h"
#include "../Help/Base/Structures/class.DynArray.h"
#include "../Help/Base/Sorting/Quick.h"
#include "../Help/Shapes/Shapes.h"
#include <assert.h>

//+ Interfacepre spravovanie zoznamu entit
class Entities : public DynArray<Entity::Base*>
{
public:
	void Draw() {
		for(UINT ent=0; ent < GetSize(); ent++) {	
			GetAt(ent)->DebugDraw();
			GetAt(ent)->Draw();
		}
	}
	float* DistanceList(vec origin) {
		// Daj zoznam vzdialenosti
		UINT i, pocet = GetSize();
		if(!pocet) assert(0);

		vec vektor;
		float* distances = new float[pocet];
		for(i = 0; i < pocet; i++ ) {
			vektor = origin - GetAt(i)->getShape()->getOrigin();
			distances[i] = vektor.Length2();
		}
		return distances;
	}
	/*
	Entities* Test(Shapes::Base* objekt) {
		Entities* list = new Entities;
		for(UINT i = 0; i < GetSize(); i++) {
			if(GetAt(i)->getShape()->Test(objekt)  > 0) { // Test nemame pre Shapes::Base
				list->Add(GetAt(i));
			}
		}
		return list;
	}
	*/
	Entities* Find(const char *name) {
		Entities* list = new Entities;
		for(UINT i=0; i < GetSize(); i++) {
			if(strcmp(GetAt(i)->getName(), name) == 0) {
				list->Add(GetAt(i));
			}
		}
		return list;
	}
	void Order(vec origin) {
		// Zorad cely zoznam podla vzdialenosti k bodu ;)
		float* distances = DistanceList(origin);
		QuickSort(GetData(), distances, 0, GetSize()-1);
		delete[] distances;
	}
	~Entities() {
		RemoveAll();
	}
};