#ifndef T_ModelTest
#define T_ModelTest

#include "./Global.h"
#include "./Direct.h"
#include "../Base/class.Scene.h"
#include "./classes/Render/Base/class.IXAnimator.h"

class ModelTest : public Scene
{	
	public :
	ModelTest();
	~ModelTest();
	PLUGIN Frame(double d);
	bool Precache();
	PLUGIN Init();

	void HudText(float x, float y, string txt, D3DCOLOR color, int size);
	PLUGIN ModelRender();

	int gModelId;
	int gNumAnimationSets;
	int gCurrentAnimationSet;
	float kTransitionTime;
	DWORD kTimeBeforeNextAnimation;
	DWORD lastUpdateTime;

	void ModelRender(double d);
	void AnimationTiming();
	void AnimationStatus();
};

#endif