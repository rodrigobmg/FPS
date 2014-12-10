#pragma once
#include "../Help/Logic/QuadTree/class.QuadTreeManager.h"
#include "../Help/App/class.Game.h"
#include "../Help/Logic/Map/class.Map.h"
#include "../Help/Logic/Camera/class.Camera.h"

class World : 
	public QuadTreeManager,
	public Module
{
private:
	// Vsetke entity su zaroven v QuadTree manageri
	Entities m_outofworld;
	Entities m_movedentities;
	Camera::Camera* m_camera;
	Map::Base* m_map;

	void ResetEntities();

protected:
	virtual void Frame(SFrame& s);

public:	
	World();
	~World();
	
	// Pomocky
	Camera::Camera* getCamera();
	void setCamera(Camera::Camera* camera);
	Map::Base* getMap();
	void setMap(Map::Base* map);
	Entities* getOutListEntities();
	Entities* getWorldEntities();

	// *as well as neighbourhood searches. - to riesi Shape Sphere a Test co vracia Entities
};
