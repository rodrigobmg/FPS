#ifndef H_BaseOCTree
#define H_BaseOCTree
#include "../../Shapes/Shapes.h"
using namespace Shape;

// Pametovy exponencialny rast (2^n)+1
template<typename TYPE>
class BaseOCTree : protected AABBMM
{
protected :
	TYPE* m_next[2];
	TYPE* m_parent;

	void Set(TYPE* parent = NULL) { // na dedenie aj zaciatok
		m_next[0] = NULL;
		m_next[1] = NULL;
		m_parent = parent;
	}

public : 
	void AddLevel();
	bool DeleteLevel();	
	void DeleteAll();
	void Render();
	DWORD getCount();

	~BaseOCTree() {
		DeleteAll();
	}
	BaseOCTree* getChild(int i) { return m_next[i]; }
	BaseOCTree* getParent() { return m_parent; }
};

template<typename TYPE>
inline DWORD BaseOCTree<TYPE>::getCount() { 
	DWORD pocet = 1;
	if(m_next[0] != NULL) pocet += m_next[0]->getCount();
	if(m_next[1] != NULL) pocet += m_next[1]->getCount();
	return pocet;
}

template<typename TYPE>
inline void BaseOCTree<TYPE>::Render()  {	
	// Renderuj aj deticky ...
	Base::Draw();
	if(m_next[0] != NULL) m_next[0]->Render();
	if(m_next[1] != NULL) m_next[1]->Render();
}

template<typename TYPE>
inline bool BaseOCTree<TYPE>::DeleteLevel() {
	if(m_next[0] != NULL && m_next[1] != NULL) {
		m_next[0]->DeleteLevel();
		m_next[1]->DeleteLevel();
		return true;
	}
	if(m_next[0] != NULL) {
		if(!m_next[0]->DeleteLevel()) {
			delete m_next[0];
			m_next[0] = NULL;
		}
		return true;
	}
	if(m_next[1] != NULL) {
		if(!m_next[0]->DeleteLevel()) {
			delete m_next[1];
			m_next[1] = NULL;
		}
		return true;
	}
	return false;
}

template<typename TYPE>
inline void BaseOCTree<TYPE>::DeleteAll() {
	SAFE_DELETE(m_next[0]);
	SAFE_DELETE(m_next[1]);
}

template<typename TYPE>
inline void BaseOCTree<TYPE>::AddLevel() {
	if(m_next[0] != NULL || m_next[1] != NULL) {
		if(m_next[0] != NULL) m_next[0]->AddLevel();
		if(m_next[1] != NULL) m_next[1]->AddLevel();
		return;
	}
	
	// X rozdelime na 4 male casti
	vec extend, origin2;
	origin2 = getOrigin();
	extend = getAbsMax() - getAbsMin();
	extend.x *= 0.25;
	
	// TODO Skontrolovat, asi zle, prepisat na rychlejsie.
	m_next[0] = new TYPE;
	m_next[1] = new TYPE;

	origin2.x -= extend.x;
	m_next[0] = Set(this);
	m_next[0]->setSize(origin2, extend);
	m_next[1] = Set(this);
	origin2.x += 2.0f * extend.x; // a vrat sa na druhu stranu, teda o 2 taku vzdialenost
	m_next[1]->setSize(origin2, extend);
}

#endif