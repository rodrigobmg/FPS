#define lol
#ifndef lol

#include "../../Global.h"
#include "./classes/AI/class.CMapSquare.h"
#include "./classes/AI/class.CBinaryHeap.h"
#include "./classes/AI/class.CPathResult.h"

/*
	iPath* findShortestPathTo(const cPosition& target) const;
	iPath* findShortestPathTo_IgnoringOneObject(const cPosition& target, cGameObject& toIgnore) const;
	iPath* findPathAway(const cPosition& awayFrom, long distanceAway) const;
	iPath* findStraightLinePathTo(const cPosition& target) const;
*/

class CPath : public CMapSquare
{
public :
	DynArray<CPath*> openlist; // int openlist[8]; // parentov nepotrebujeme, toto uz je pretriedene
	CBinaryHeap * closelist; // 
	
	CPath * end;
	CPath * called_from;
	CPath * calling;
	DynArray<CPath*> paths;
	int binaryat; // toto sa stale inkrementuje
	static long cyklov;
	float size;


	CPath();
	~CPath();
	void Default();
	void DojdiK(CPath * a);
	void HladajDalsie(); 
	CPathResult Make();
	void Print();
	
protected:
	void MakeID(); // spravy nam ID
	void GetParents();
	bool ProblematicSituations();
	void PrefilterCollision();
	void PrefilterParentOpenList();
	void Ohodnot();
	void SortPaths();
};

long CPath::cyklov = 0;

#endif