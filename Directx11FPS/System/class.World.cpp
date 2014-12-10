#include "class.World.h"

World::World() : QuadTreeManager(vec(0.f), vec(25000.f, 10000.f, 25000.f), 5) {
	// Nastav defaultne hodnoty
	m_map = 0;
	m_camera = 0;
}
World::~World() {
	SAFE_DELETE(m_map)
}
Camera::Camera* World::getCamera() { 
	return m_camera; 
}
void World::setCamera(Camera::Camera* camera) { 
	m_camera = camera;
}
Map::Base* World::getMap() { 
	return m_map; 
}
void World::setMap(Map::Base* map) { 
	m_map = map;
}
Entities* World::getOutListEntities() {
	return &m_outofworld;
}
Entities* World::getWorldEntities() {
	return &m_entities;
}
void World::ResetEntities() {
	QuadTree* oc;
	Entity::Base *e;
	for(UINT i=0; i < m_movedentities.GetSize(); i++) {
		e = m_movedentities.GetAt(i);
		oc = QuadTreeManager::Find(e->getShape());
		if(oc == NULL) {
			m_outofworld.Add(e);
		} else {
			oc->getEntities()->Add(e);
		}
	}
}
void World::Frame(SFrame& s) {
	// IQ
	ResetEntities();
	if(m_camera != NULL) { 
		if(m_camera->hasMoved()) {
			TestVisibility(m_camera->getFrustum()); 
		}
		m_camera->Frame( s.GetDeltaTime() );
		m_map->Render( m_camera->Base::GetViewMatrix() );
	}

	// Renderuj dalsie veci
	QuadTreeManager::DrawSustavu(100.0f);
	QuadTreeManager::Render();
}
