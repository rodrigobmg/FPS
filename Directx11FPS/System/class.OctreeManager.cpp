#include "class.Managers.h"

/*
	// najprv sa tstuje rychle AA poda toho sa priradi do Octree
	// Octree sa porovnava referencia
	public bool AABox(); // porovnava priemery, priorita 1
	public bool Circle(); // porovnava priemery, priorita 1
	public bool BBBox(); // z uhlom, priorita 2
	public bool SubParticles(; // ak je to lietadlo tak subcasti porovna, casto krat nepouzijeme priorita 3.
	public virtual bool SubParticles() { return true; }
*/

void OCtreeManager::ResetEntities() {
	// Entities* movedentities;
}

void OCtreeManager::Frame() { 
	ResetEntities(); 
	OCTree::Frame(); 
}