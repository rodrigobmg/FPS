#include <windows.h>
#include "../../Global.h"
#include "../Base/System/class.Timer.h"
#include "class.GameConfig.h"
#include "class.Game.h"
#include <string>
using namespace std;

Game::Game() {
	// Startujeme hru 
	m_goodframes = 0;
	m_badframes = 0;
}
void Game::Start() {
	App::Start();
	m_frametime.Start();
	m_gametime.Start();
}
bool Game::Run() 
{	
	double delta = m_frametime.GetTime();
	if(!LimitFPS(delta)) return false;
	
	// Statistyky
	if(delta > m_casovac*2.0f) {
		m_badframes++;
	} else {
		m_goodframes++;
	}

	// Frame
	m_deltatime = GetExactlyDeltaTime();
	GameFrame();
	m_gametime.Reset();
	m_frametime.Reset();
	return true;
}
bool Game::LimitFPS(double delta) {
	// Limitujeme fps
	if(m_limitfps && (delta < m_casovac)) { // Povolime limitovanie FPS ?
		if(m_oneskorenie) {
			DWORD sleep = (DWORD)  (m_casovac - delta) * 1000.0f;
			Sleep(sleep);
		} else {
			return false; // pockame klasickou cestou
		}
	}
	return true;
}
void Game::GameFrame() {
	//++  PreFrame
	FPSCount();

	//++ Frame - vyskladame si co posleme do kazdeho frame
	RunModules( *((SFrame*)this) );

	//++ PostFrame
	if(DEBUG) {
		//printf("FPS %d\n", this->GetFPS());
		//PrintStats();
	}
}
void Game::Stop() {
	ModulesOff();
	App::Stop();
}



bool Game::PresloSekund(double sec, double &cas) {
	if( cas + sec > m_gametime.GetAbsolute() ) {
		cas = m_gametime.GetAbsolute();
		return true;
	}
	return false;
}
double Game::GetGlobalTime() {
	return m_gametime.GetAbsolute();
}
double Game::GetExactlyDeltaTime() {
	return m_gametime.GetTime();
}
double Game::GetDeltaTime() {
	return m_deltatime;
}
Game::~Game() {
	ModulesOff();	
}
