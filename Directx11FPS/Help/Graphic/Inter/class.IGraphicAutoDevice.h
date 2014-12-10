#pragma once
#include "../../App/class.Direct.h"

extern Direct* gDirect;

/*
	Potom mozme vytiahnut zoznam grafickych objektov ktore su nacitane.
*/

class IGraphicAutoDevice //: public IGraphicDevice 
{
protected:
	inline Direct* Direct() { return gDirect; }
};	