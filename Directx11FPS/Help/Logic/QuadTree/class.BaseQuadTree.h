#ifndef H_BaseQuadTree
#define H_BaseQuadTree
#include "../../Shapes/Shapes.h"

// Pametovy exponencialny rast (4^n)+1
template<class TYPE>
class BaseQuadTree : protected Shapes::AABBMM
{
private:
	friend TYPE;
	TYPE* m_next[4];
	TYPE* m_parent;
	DWORD m_level;

	inline void ChildrenNULL() {
		m_next[0] = NULL;
		m_next[1] = NULL;
		m_next[2] = NULL;
		m_next[3] = NULL;
	}
	inline void ChildrenDELETE() {
		delete m_next[0];
		delete m_next[1];
		delete m_next[2];
		delete m_next[3];
	}
	void NewLevel();
	void DivX(vec& absmin, vec& absmax, vec& origin);
	void DivY(vec& absmin, vec& absmax, vec& origin);
	void DivZ(vec& absmin, vec& absmax, vec& origin);
	void DivSimetric();

protected :
	virtual void AfterLevel() = 0;
	inline void SetParent(TYPE* parent = NULL) {
		m_parent = parent;
	}

public : 
	void AddLevel();
	bool DeleteLevel();	
	void DeleteAll();
	DWORD getCount();
	
	BaseQuadTree() { 
		ChildrenNULL(); 
		SetParent(); 
	}
	~BaseQuadTree() { DeleteAll(); }
	inline TYPE* getChild(int i) { return m_next[i]; }
	inline TYPE* getParent() { return m_parent; }
	inline bool hasChildren() { return m_next[0] != NULL; }
	inline DWORD getLevel() { return m_level; }
};

template<class TYPE>
DWORD BaseQuadTree<TYPE>::getCount() { 
	DWORD pocet = 1;
	if(hasChildren()) {
		pocet += m_next[0]->getCount();
		pocet += m_next[1]->getCount();
		pocet += m_next[2]->getCount();
		pocet += m_next[3]->getCount();
	}
	return pocet;
}

template<class TYPE>
bool BaseQuadTree<TYPE>::DeleteLevel() {
	if(hasChildren()) {
		bool x = FALSE;
		if(m_next[0]->DeleteLevel()) { x = true; }
		if(m_next[1]->DeleteLevel()) { x = true; }
		if(m_next[2]->DeleteLevel()) { x = true; }
		if(m_next[3]->DeleteLevel()) { x = true; }

		// Ak aspon jedno dieta ma dieta tak true
		if(x) return true;
		
		// Inak vymaz deti
		ChildrenDELETE();
		return false;
	}
	return false;
}

template<class TYPE>
inline void BaseQuadTree<TYPE>::DeleteAll() {
	if(!hasChildren()) return;
	ChildrenDELETE();
	ChildrenNULL();
}

template<class TYPE>
void BaseQuadTree<TYPE>::AddLevel() {
	// Ak uz ma deti
	if(hasChildren()) {
		m_next[0]->AddLevel();
		m_next[1]->AddLevel();
		m_next[2]->AddLevel();
		m_next[3]->AddLevel();
		return;
	}
	NewLevel();
	AfterLevel();
}

template<class TYPE>
void BaseQuadTree<TYPE>::DivZ(vec& absmin, vec& absmax, vec& origin) {
	// Po Z suradnici delenie
	m_next[0]->setSizeMM(vec(origin.x, absmax.y, absmin.z),		vec(absmax.x, origin.y, absmax.z));
	m_next[1]->setSizeMM(vec(origin.x, absmin.y, absmin.z),		vec(absmax.x, origin.y, absmax.z) );
	m_next[2]->setSizeMM(vec(absmin.x, absmin.y, absmin.z),		vec(origin.x, origin.y, absmax.z) );
	m_next[3]->setSizeMM(vec(absmin.x, origin.y, absmin.z),		vec(origin.x, absmax.y, absmax.z) );
}

template<class TYPE>
void BaseQuadTree<TYPE>::DivY(vec& absmin, vec& absmax, vec& origin) {
	// Po Y suradnice
	m_next[0]->setSizeMM(vec(origin.x, absmin.y, origin.z),		vec(absmax.x, absmax.y, absmax.z));
	m_next[1]->setSizeMM(vec(origin.x, absmin.y, absmin.z),		vec(absmax.x, absmax.y, origin.z) );
	m_next[2]->setSizeMM(vec(absmin.x, absmin.y, absmin.z),		vec(origin.x, absmax.y, origin.z) );
	m_next[3]->setSizeMM(vec(absmin.x, absmin.y, origin.z),		vec(origin.x, absmax.y, absmax.z) );
}

template<class TYPE>
void BaseQuadTree<TYPE>::DivX(vec& absmin, vec& absmax, vec& origin) {
	// Po X suradnice
	m_next[0]->setSizeMM(vec(absmin.x, origin.y, origin.z),		vec(absmax.x, absmax.y, absmax.z));
	m_next[1]->setSizeMM(vec(absmin.x, origin.y, absmin.z),		vec(absmax.x, absmax.y, origin.z) );
	m_next[2]->setSizeMM(vec(absmin.x, absmin.y, absmin.z),		vec(absmax.x, origin.y, origin.z) );
	m_next[3]->setSizeMM(vec(absmin.x, absmin.y, origin.z),		vec(absmax.x, origin.y, absmax.z) );
}

template<class TYPE>
void BaseQuadTree<TYPE>::DivSimetric() {
	vec absmin, absmax, origin, extend;
	absmin = getAbsMin();
	absmax = getAbsMax();
	origin = getOrigin();
	extend = origin - absmin;
	extend.Abs();

	if( extend.x < extend.y) {
		if( extend.x < extend.z) { // X najmensie
			DivX(absmin, absmax, origin);
		} else { // Z najmensie
			DivZ(absmin, absmax, origin);
		}
	} else {
		if( extend.y < extend.z) { // Y najmensie
			DivY(absmin, absmax, origin);
		} else { // Z najmensie
			DivZ(absmin, absmax, origin);
		}
	}
}
template<class TYPE>
void BaseQuadTree<TYPE>::NewLevel() {
	// Vytvor deti
	m_next[0] = new TYPE;
	m_next[1] = new TYPE;
	m_next[2] = new TYPE;
	m_next[3] = new TYPE;

	DivSimetric();
}

#endif