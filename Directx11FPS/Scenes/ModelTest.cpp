#include "Modeltest.h"
#include "./classes/App/class.Direct.h"
#include "./classes/App/class.Game.h"
#include "./classes/render/base/class.Draw.h"

/*
0 stoji
1 chodi
2 hybe hlavou
3 chyta
4 pada na zem
*/
ModelTest::~ModelTest() 
{

}
ModelTest::ModelTest() {
	kTransitionTime = 2500;
	kTimeBeforeNextAnimation = 4000;
	lastUpdateTime = timeGetTime();
}
PLUGIN ModelTest::Init() {
	G::Direct->animator->ChangeAnimationSet(gModelId, 1, 2500); // akokeby idle, sa hybe...
	return PLUGIN_CONTINUE;
}
PLUGIN ModelTest::Frame(double d)	
{
	//AnimationTiming();
	ModelRender(d);
	return PLUGIN_CONTINUE;
}
void ModelTest::AnimationTiming() {
	static DWORD lastTimeAnimationChanged = timeGetTime();
	
	DWORD timeElapsedSinceAnimationChange=timeGetTime()-lastTimeAnimationChanged;
	
	// Periodically advance to the next animation set
	if (timeElapsedSinceAnimationChange>kTimeBeforeNextAnimation)
	{
		gCurrentAnimationSet++;
		if (gCurrentAnimationSet>=gNumAnimationSets)
			gCurrentAnimationSet=0;

		G::Direct->animator->ChangeAnimationSet(gModelId, gCurrentAnimationSet, kTransitionTime);
		lastTimeAnimationChanged=timeGetTime();
	}
}
void ModelTest::AnimationStatus() {
	// Draw some text showing the controls and current angles and position
		
	/*int kFontSize = 14;
	D3DCOLOR fontColor = D3DCOLOR_XRGB(255,255,255);    
	RECT rct;
	rct.left = kFontSize;
	rct.right = 800;
	rct.top = kFontSize;
	rct.bottom = rct.top+kFontSize;
	string pismo; 
	
	pismo = "Model: Data\\bones_all.x";
	G::Direct->font->DrawText(NULL, pismo.c_str(), -1, &rct, 0, fontColor );
	rct.top += kFontSize; rct.bottom=rct.top+kFontSize;

	HudText(14.0, 200.0, "Animacia: "+ToString(gCurrentAnimationSet), D3DCOLOR_XRGB(255,255,255), 14);

	pismo = "FrameTime: "+ToString(G::Game->config->casovac, 15);
	G::Direct->font->DrawText(NULL, pismo.c_str(), -1, &rct, 0, fontColor );
	rct.top+=kFontSize;rct.bottom=rct.top+kFontSize;
	
	pismo = "FPS: "+ToString(G::Game->GetFPS());
	G::Direct->font->DrawText(NULL, pismo.c_str(), -1, &rct, 0, fontColor );
	rct.top+=kFontSize;rct.bottom=rct.top+kFontSize;
	*/
}
void ModelTest::ModelRender(double d)
{	
	DWORD timeElapsedSinceLastUpdate=timeGetTime()-lastUpdateTime;

	D3DXMATRIX world, matTrans, matWorld, matRotX,matRotY,matRotZ;

	// Calculate rotation matrix
	D3DXMatrixRotationX( &matRotX, 0.0 );        // Pitch
	D3DXMatrixRotationY( &matRotY, 100.0 );        // Yaw
	D3DXMatrixRotationZ( &matRotZ, 0.0 );        // Roll

	static double x = 0.0;
	x += 0.1;
	D3DXMatrixTranslation(&matTrans, x, 0.0, 0.0);
	matWorld= (matRotZ*matRotX*matRotY)*matTrans;
	//D3DXMatrixScaling(&matWorld, 30.0, 30.0, 30.0); // pre toto bud dobre ale nepohybuje sa

	G::Direct->zariadenie->SetTransform( D3DTS_WORLD, &matWorld);

	G::Direct->animator->Render(gModelId, matWorld, (float)timeElapsedSinceLastUpdate);
	lastUpdateTime = timeGetTime();
	G::Direct->zariadenie->SetTransform( D3DTS_WORLD, &world);
}
void ModelTest::HudText(float x, float y, string txt, D3DCOLOR color, int size)
{ 
	RECT rct;
	rct.left=x;
	rct.right=x;
	rct.top=y;
	rct.bottom=rct.top+size;
	G::Direct->font->DrawText(NULL, txt.c_str(), -1, &rct, 0, color );
}
bool ModelTest::Precache()
{
	if (!G::Direct->animator->LoadXFile("Resource\\Models\\bones_all.x", &gModelId)) return false;
	return true;
}
