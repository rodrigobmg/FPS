#pragma once
#include "class.BaseQuadTree.h"
#include "../../../Entity/class.Entities.h"

class QuadTree : 
	public BaseQuadTree<QuadTree>
{
protected:
	// Dalsia implementacia
	friend class QuadTree;
	Entities m_entities; // TODO len tie presne v tej oblasti, ak su medzi tak su v parente
	bool m_isvisible;
	void NotVisible();
	void AfterLevel();

public:
	Entities* getEntities() {
		return &m_entities;
	}

	QuadTree* Find(Shapes::Base *s); // Snazime sa najst najmensi QuadTree ktory sa bude dotykat
	void Render();
	void TestVisibility(Shapes::Frustum* frustum); // Otestuj ci vidime tento QuadTree


};