#define lol
#ifndef lol


//#ifndef H_CMapZones
//#define H_CMapZones
#include "../../Global.h"
#include "./classes/AI/class.CBinaryHeap.h"
#include "./classes/AI/class.CMapSquare.h"
// http://www.policyalmanac.org/games/binaryHeaps.htm

class CMapSquare {
public:
	BBox box;
	bool bad;
	int score[3]; // 0-S, 1-G, 2-F=S+G
	int id[3]; // 0-x , 1-y
};

class CMapZone	// Zatial v 2D
{
public :
	CMapZone();
	~CMapZone();

	static void DebugZone(); // potrebujeme na debug
	//static PLUGIN Preprocess(BBox *a, float velkost);
	static bool TestCollision(CMapSquare * a); // vracia ID s koliziou, inak -1
	static DynArray <CMapSquare> blocks; // skutocne data o CMapSquare	
};

#endif