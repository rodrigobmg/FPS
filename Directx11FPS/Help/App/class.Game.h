#ifndef H_Game
#define H_Game
#include <windows.h>
#include <string>
#include <vector>
#include "../../Global.h"
#include "class.App.h"
#include "class.GameConfig.h"
#include "Base/class.EasyFPS.h"
#include "../Base/class.Module.h"

class Game : 
	public App, 
	public Modules, 
	public GameConfig,
	public EasyFPS,
	public SFrame
{
public:
	Game();
	~Game();

	double GetGlobalTime();
	double GetDeltaTime();
	double GetExactlyDeltaTime();
	bool PresloSekund(double sec, double &cas);
	Screen& getScreenInfo() {
		return (**App::getScreenInfo());
	}

	virtual void Initialize() = 0;
	virtual void Stop();
	virtual void Start();

protected:
	virtual bool Run();

private :
	bool LimitFPS(double delta);
	void GameFrame();

	Timer m_frametime;	// iba direktor riadi cas ...
	Timer m_gametime;	// cas pre hru, da sa pozastavit ...
	
	double m_deltatime;	// rozdiel casu pocas snimkov ...
	long int m_goodframes; // dobre snimky
	long int m_badframes; // snimky ktore nestihli
};

extern Game* gGame;


#endif

