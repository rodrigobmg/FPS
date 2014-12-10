/*
#include "class.InputManager.h"
#include "./Direct.h"
#include "./classes/UI/Base/class.Input.h"
#include "../Help/class.GameConfig.h"
#include "../Help/Base/class.WinDebug.h"
#include "../Help/Algebra/class.Math.h"
#include "../Help/Base/class.Timer.h"
#include "../Entity/Base/class.Ent.h"
#include "../Entity/Base/class.Player.h"
#include "./classes/APP/class.Game.h"
#include "./classes/APP/class.Sound.h"
#include "../Help/Base/class.Octree.h"
#include "./classes/render/class.RenderManager.h"

void Napis(int x, int y, char const *text);
extern int PARTICIPATE_TEST;

void InputManager::Frame() 
{
	// A nejake dalsie prvky ...j
	static float uhol;
	if( Input::GetRozdielX()!=0) {
		uhol = D3DXToRadian( (float) Input::GetRozdielX() / G::Game->config->kamera_citlivost[1]);
		PohybKamerou(false, uhol);
	}
	if( Input::GetRozdielY()!=0) {
		uhol = D3DXToRadian( (float) Input::GetRozdielY() / G::Game->config->kamera_citlivost[1]);
		PohybKamerou(true, uhol);
	}
	Klavesnica();
}
PLUGIN InputManager::Klavesnica()
{
	// Koniec ???
	if( Input::Press(VK_ESCAPE)) {
		G::Game->End();
		return PLUGIN_HANDLE;
	}
	CAN_CONTINUE( KlavesnicaVhre() );
	return PLUGIN_CONTINUE;
}
PLUGIN InputManager::KlavesnicaVhre() 
{
	static float uhol;

	// Pohyby dopredu / dozadu
	if( Input::Keydown('W')) {
		gPlayers[ENT_CAMERA].construct.origin.x += cos(gPlayers[ENT_CAMERA].uhol.x) * G::Game->config->pohyb_citlivost[0];
		gPlayers[ENT_CAMERA].construct.origin.z += sin(gPlayers[ENT_CAMERA].uhol.x) * G::Game->config->pohyb_citlivost[0];
			//debug.echo(__FUNCTION__);
			//debug.echo(gPlayers[ENT_CAMERA].construct.origin);
	}
	if( Input::Keydown('S')) {
		gPlayers[ENT_CAMERA].construct.origin.x -= cos(gPlayers[ENT_CAMERA].uhol.x) * G::Game->config->pohyb_citlivost[1];
		gPlayers[ENT_CAMERA].construct.origin.z -= sin(gPlayers[ENT_CAMERA].uhol.x) * G::Game->config->pohyb_citlivost[1];
			//debug.echo(__FUNCTION__);
			//debug.echo(gPlayers[ENT_CAMERA].construct.origin);
	}

	// Bocne pohyby ....
	if( Input::Keydown('A')) {
			uhol = PI/4.0f + gPlayers[ENT_CAMERA].uhol.x; // 90 stupnov - alfa			
			gPlayers[ENT_CAMERA].construct.origin.x += cos(uhol) * G::Game->config->pohyb_citlivost[2] * 40.0f;
			gPlayers[ENT_CAMERA].construct.origin.z += sin(uhol) * G::Game->config->pohyb_citlivost[2] * 40.0f;
			///bug.echo(__FUNCTION__);
			//debug.echo(gPlayers[ENT_CAMERA].construct.origin);
	}	
	if( Input::Keydown('D')) {
			uhol = PI/4.0f + gPlayers[ENT_CAMERA].uhol.x; // 90 stupnov - alfa			
			gPlayers[ENT_CAMERA].construct.origin.x -= cos(uhol) * G::Game->config->pohyb_citlivost[2] * 40.0f;
			gPlayers[ENT_CAMERA].construct.origin.z -= sin(uhol) * G::Game->config->pohyb_citlivost[2] * 40.0f;
			//debug.echo(__FUNCTION__);
			//debug.echo(gPlayers[ENT_CAMERA].construct.origin);
	}
	
	// Pohyby s kamerou
	if( Input::Keydown(VK_RIGHT)) {
		PohybKamerou(false, -1.0*G::Game->config->kamera_citlivost[0]);
	}
	if( Input::Keydown(VK_LEFT)) {
		PohybKamerou(false, G::Game->config->kamera_citlivost[0]);
	}
	if( Input::Keydown(VK_DOWN)) {
		PohybKamerou(true, -1.0*G::Game->config->kamera_citlivost[0]);
	}
	if( Input::Keydown(VK_UP)) {
		PohybKamerou(true, G::Game->config->kamera_citlivost[0]);
	}

	
	
	if( Input::Keydown('N')) {
		G::Game->render->RenderSpecialMatrices = true;
	} else {
		G::Game->render->RenderSpecialMatrices = false;
	}

	if( Input::Press('G')) {
		PLUGIN_MSG("Start loading: music.mp3");
		G::Sound->builder->RenderFile(L"data\\music.mp3", NULL);
		G::Sound->control->Run();	
		//sound_control->Stop();	
	}
	if( Input::Press('T')) {
		PLUGIN_MSG("Start loading: mom.wav");
		G::Sound->builder->RenderFile(L"data\\mom.wav", NULL);
		G::Sound->control->Run();	
		//sound_control->Stop();	
	}	
	if( Input::Press('J')) {
		G::Sound->control->Stop();
	}
	if( Input::Press('H')) {
		PLUGIN_MSG("Start loading: crow.wav");
		G::Sound->builder->RenderFile(L"data\\crow.wav", NULL);
		G::Sound->control->Stop();
		G::Sound->control->Run();	
		//sound_control->Stop();	
	}

	if( Input::Press('F')) {
		G::App->debug->echo("Stlacene F");		
		// skuska
		double lol1;
		float lol2;
		double lol3;
		Timer casovac;
		casovac.LimitThreadAffinityToCurrentProc();
		lol1 = casovac.GetTime();
		lol2 = casovac.GetElapsedTime();
		lol3 = casovac.GetAbsoluteTime();
		printf("\n\ncasovac %10f\n", lol1 );
		printf("casovac %10f\n", lol2 );
		printf("casovac %10f\n", lol3 );
		casovac.GetTimeValues(&lol1, &lol3, &lol2 );
		printf("casovac %10f\n", lol2);
	}
	
	if( Input::Press('M')) {
		if( PARTICIPATE_TEST == 4 ) {
			PARTICIPATE_TEST = 0;
			printf("Participate test: %i\n", PARTICIPATE_TEST);
		} else {
			PARTICIPATE_TEST++;
			printf("Participate test: %i\n", PARTICIPATE_TEST); 
		}
	}

	if( Input::Press('O')) {
		OC->Add();
		printf("OCtree stvorcov: %i\n", OC->CelkovyPocet());
	}
	if( Input::Press('P')) {
		OC->Set(0);
		printf("OCtree resetovany");
	}
	return PLUGIN_CONTINUE;
}
void InputManager::PohybKamerou(bool vertikal, float rotacia)
{
	//debug.echo(__FUNCTION__);
	//debug.echo(gPlayers[ENT_CAMERA].construct.origin);
	if(vertikal) {
		if( ( gPlayers[ENT_CAMERA].uhol.y < Math::UholNaRadiany(90) && rotacia > 0) || (gPlayers[ENT_CAMERA].uhol.y > Math::UholNaRadiany(-90) && rotacia < 0) ) {
			gPlayers[ENT_CAMERA].uhol.y += rotacia;
		}
		//if(g_Camera.Rotation.v > PI*2) {
		//	g_Camera.Rotation.v = g_Camera.Rotation.v-PI*2;
		//}
	} else {
		gPlayers[ENT_CAMERA].uhol.x += rotacia;
		if(gPlayers[ENT_CAMERA].uhol.x > PI*2) {
			gPlayers[ENT_CAMERA].uhol.x -= PI*2;
		}
	}
}
*/