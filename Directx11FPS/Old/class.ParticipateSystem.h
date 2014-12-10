#ifndef H_Participate
#define H_Participate

#include "./Global.h"
#include "class.DynArray.h"
#include "./classes/Help/Base/class.BBox.h"

#pragma region // Participate System

class ParticipateBod {
public:
	ParticipateBod();
	BBox structure;
	vec velocity;
};

extern float gravity;

class Participate
{
public:
	Participate();
	Participate(int i);
	~Participate();
	
	void Set(int i); // volame najskor a poyom nahodne rozmiestnime
	void RandomizeInBox();
	void Render(double time);
	
	BBox box;
	bool gravitacia;
	
	void Participate::Test0(double cas);
	void Participate::Test1(double cas);
	void Participate::Test2(double cas);
	void Participate::Test3(double cas);
	void Participate::Test4(double cas);

	ParticipateBod * bod;
	int bodsize;
private:
	void Physics(double timedelta);
	void RandomizeItem(int i);
	void Collision();
};


#pragma endregion

#endif