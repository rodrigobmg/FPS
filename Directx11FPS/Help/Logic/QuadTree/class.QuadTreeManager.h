#pragma once
#include "../Help/Math/class.vec.h"
#include "../Help/App/Base/class.WinDebug.h"
#include "class.QuadTree.h"

class QuadTreeManager : public QuadTree
{
public:
	QuadTreeManager(vec &absmin, vec &absmax, int levels) { 
		// Vytvor podcasti
		WinDebug::getInstance();
		setSizeMM(absmin, absmax);

		for(int i=0; i < levels; i++) { // exponencialny rast 2^n+1 , maximalne 22
			this->AddLevel(); // zasekava sa STACK ?
			printf("Pocet %d\n", this->getCount());
		}
	}
};
