#include "class.QuadTree.h"

void QuadTree::AfterLevel() {
	getChild(0)->SetParent(this);
	getChild(1)->SetParent(this);
	getChild(2)->SetParent(this);
	getChild(3)->SetParent(this);
}

// Snazime sa najst najmensi QuadTree ktory sa bude dotykat
QuadTree* QuadTree::Find(Shapes::Base *s) {
	// Nieje v kolizii
	if(!s->Test(this)) return NULL;

	// Hladaj este mensi Quadtree
	{
		if(!hasChildren()) return this;

		// Objekt je medzi viacerymi Quadtree ?
		QuadTree* child;
		DWORD medzi = false;
		for(DWORD i=0; i < 4; i++) {
			child = getChild(i)->Find(s);
			if(child == NULL) continue;
			if(medzi) return this; // Je
			medzi = true;
		}
			
		// Nieje medzi viacerymi objektamy...
		return child;
	}

	// Nenaslo vrat seba
	return this;
}
void QuadTree::Render() {
	// Je viditelny ?
	//if(!m_isvisible) return;
		
	// Renderuj vsetke entity
	m_entities.Draw();
	Base::Draw(); // TODO debug sa ma tiez vykreslovat ?

	// Rekurzivne dalej
	
	// TODO allows you to render your octree nodes in front to back order. 
	// *I currently went just with ordering my vbos by distance to the camera
	// *vypluvni zoznam octree a tie sa potom zoradia

	if(!hasChildren()) return;
	// Renderuj aj deticky ...
	/*getChild(0)->setColor( vec4(1.0f, 0.0f, 0.0f, 1.0f) );
	getChild(1)->setColor( vec4(0.0f, 1.0f, 0.0f, 1.0f) );
	getChild(2)->setColor( vec4(0.0f, 0.0f, 1.0f, 1.0f) );
	getChild(3)->setColor( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	*/

	getChild(0)->Render();
	getChild(1)->Render();
	getChild(2)->Render();
	getChild(3)->Render();
	
}

// Otestuj ci vidime tento QuadTree
// *Nebude sa pytat na frustrum ale to sa nahodi cez parameter nech mame moznost vizualizacie
void QuadTree::TestVisibility(Shapes::Frustum* frustum) {
	if(frustum->Test(this)) {
		m_isvisible = true;
			
		// Rekurzivne
		if(hasChildren()) {
			getChild(0)->TestVisibility(frustum);
			getChild(1)->TestVisibility(frustum);
			getChild(2)->TestVisibility(frustum);
			getChild(3)->TestVisibility(frustum);
		}
	} else {
		if(m_isvisible) {
			NotVisible();
		}
	}
}
void QuadTree::NotVisible() {
	m_isvisible= false;

	// Rekurzivne
	if(hasChildren()) {
		getChild(0)->NotVisible();
		getChild(1)->NotVisible();
		getChild(2)->NotVisible();
		getChild(3)->NotVisible();
	}
}
