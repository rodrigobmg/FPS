#ifndef H_ParticipateTest
#define H_ParticipateTest
#include "../Base/class.Scene.h"
extern class Participate;

class ParticipateTest : public Scene
{	
	public :
	ParticipateTest();
	~ParticipateTest();
	PLUGIN Frame(double d);
	PLUGIN Init();

	Participate * parts;
};

#endif
