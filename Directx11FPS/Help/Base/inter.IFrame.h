#ifndef H_IFrame
#define H_IFrame
#include "../../Global.h"

class SFrame  {
public:
	virtual double GetDeltaTime() = 0;
};

class IFrame  {
protected:
	virtual void Frame(SFrame& s) = 0;
	// TODO: PreProcessFrame() - veci ktore sa daju spustit pred Frame pre multythreading
	// To iste aj PostProcessFrame()
};

#endif