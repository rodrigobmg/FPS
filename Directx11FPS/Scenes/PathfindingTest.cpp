#define lol
#ifndef lol

#include "PathFindingTest.h"

PathfindingTest::PathfindingTest() {
	object = new CPathFinding();
}
PLUGIN PathfindingTest::Init() {
	PLUGIN_MSG("PathFinding hladam cestu.");
	object->StartSearch();
	object->sort->Print();
	return PLUGIN_CONTINUE;
}
PLUGIN PathfindingTest::Frame(double d) {
	object->DebugZone();
	object->Print();
	return PLUGIN_CONTINUE;
}
PathfindingTest::~PathfindingTest() {
	SAFE_DELETE(object);
}
#endif