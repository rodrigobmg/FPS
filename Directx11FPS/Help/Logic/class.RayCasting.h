#pragma once
#include "../Entity/class.Entities.h"
#include "../Help/Shapes/Shapes.h"
#include "../Help/Math/Math.h"
#include "../Help/Base/QuickDepthSort.h"

/*
	If you're unsure what raycasting is, it's basically a collision detection routine. 
	You project a ray (line segment) into your physics world. 
	You then collect every object that this ray intersects. 
	You ignore the insitgator object (you don't want the trace hitting the AI doing the trace). 
	So basically it's a series of "does this line intersect this collision geometry" checks. 
	A good physics system optimizes this with spatial partitioning trees or 
	whatever so you don't check the line against every object in the scene.

	* Ray-Casting je asichronny
	- Teda sa poziada, posle pointer na funkciu ktora sa ma splnit a cele to bezi na novom vlakne
	- Po vykonani sa spusti dana funkcia
*/

class IRayCasting {
private:	
	RayCasting() { // abstract class
		// Registruj tento objekt 
		
		
		//a vytvor nove vlakno

	} 
	void Finalize() {
		// Koniec castingu
		// Vymaz tento obkejt
		delete this;
	}

protected:
	void Test(Entities *testlist, Entities *okremlist, Ray *ray) {
		UINT i, j;
		Entity *a;
		float distance;
		for(i=0; i < testlist->GetSize(); i++) {
			a = testlist->GetAt(i);
			if( okremlist->Contains(a) ) continue;
			if(a->getShape()->Test(ray, &distance)) {
				if(Casting(a, &distance)) break;
			}
		}
		Finalize();
	}
	void Test(Entities *testlist, Entity *okrem, Ray *ray) {
		UINT i, j;
		Entity *a;
		float distance;
		for(i=0; i < testlist->GetSize(); i++) {
			a = testlist->GetAt(i);
			if( a == okrem ) continue;
			if(a->getShape()->Test(ray, &distance)) {
				if(Casting(a, &distance)) break;
			}
		}
		Finalize();
	}

	// co sa spravit s vysledkom ? ;)
	// true je break
	virtual bool Casting(Entity *ent, float *distance) = 0; 
};


class IOrderedRayCasting : public IRayCasting {
private:
	vec m_ab;
	vec m_a;
	float m_d;

	// Metoda prebrata z Math, trocha vylepsena
	float ClosestPointOnLine(const vec &point) {
		float t = Math::DOT3(m_ab, point - m_a);	// ab is unit vector, t is distance from a to point projected on line ab
		if(t< 0) return 0;		// point projected on line ab is out of line, closest point on line is a
		if(t>= m_d) return 1;		// point projected on line ab is out of line, closest point on line is b
		return t;				// point away from a on t length in direction ab
	}

	void Order(Entities *testlist, Ray *ray) {
		m_ab = ray->getV();
		m_ab.Normalize();
		m_d = m_ab.Length();
		m_a = ray->getA();

		// Distance list
		UINT i, pocet = testlist->GetSize();
		float* distances = new float[pocet];
		for(i = 0; i < pocet; i++ ) {
			distances[i] = ClosestPointOnLine( testlist->GetAt(i)->getShape()->getOrigin() );
		}
		QuickDepthSort(testlist->GetData(), distances, 0, pocet-1);
	}

protected:
	void Test(Entities *testlist, Entities *okremlist, Ray *ray) {
		Order(testlist, ray);
		Test(testlist, okremlist, ray); 
	}
	void Test(Entities *testlist, Entity *okrem, Ray *ray) {
		Order(testlist, ray);
		Test(testlist, okrem, ray); 
	}
};