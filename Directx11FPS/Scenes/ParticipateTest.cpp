#include "./classes/App/class.Game.h"
#include "ParticipateTest.h"
#include "./classes/Help/Base/class.ParticipateSystem.h"

ParticipateTest::ParticipateTest() 
{
	parts = new Participate();
}
PLUGIN ParticipateTest::Init() {
	parts->gravitacia = true;
	parts->box.SetSize(vec(0.0, 0.0, 0.0) ,
					vec(-200.0f, -200.0f, 200.0f) ,
					vec(200.0f, 200.0f, -200.0f) );
	parts->Set(3000);
	parts->RandomizeInBox();
	PLUGIN_MSG("Participate TEST online.");
	return PLUGIN_CONTINUE;
}
ParticipateTest::~ParticipateTest() {
	SAFE_DELETE(parts);
}
PLUGIN ParticipateTest::Frame(double d) {
	parts->Render(d);
	return PLUGIN_CONTINUE;
}
