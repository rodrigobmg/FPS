#ifndef H_GameConfig
#define H_GameConfig
#include <string>
#include <vector>

// TODO *saver - schopnost ulozit status celej hry / status vsetkych premennych v frame to suboru a neskor nacitat toto by pomohlo debugu a neskor je to dobry save mananger

class GameConfig 
{
private:
	std::string m_file;

protected:
	virtual void CMDParser(std::vector<std::string> parameters) = 0;

public:
	bool m_debug;									// mame debug zapnuty ?
	std::string m_name;
	bool m_limitfps;
	float m_casovac;								// kolko milisekund ma na scenu ?
	bool m_oneskorenie;								// pri casovani pouzivame metodu oneskorenia ? - komplexne setri CPU ale za to brzdi celu aplikaciu
	float m_pohyb_citlivost[2];
	float m_kamera_citlivost[2];					// SIPKY, MYSKA
	bool m_mouse_inverze;							// Opacne mysku
	int m_rozlisenie[2];

	GameConfig();
	virtual void Defaultne();
	bool SaveConfig();
	bool LoadConfig();
};

#endif
