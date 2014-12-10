#ifndef H_EntitaPlayer
#define H_EntitaPlayer

#include <Global.h>
#include "../EntConst.h"
#include "class.Ent.h"

#pragma region 
class cPlayer : public Entita // dalsia specificka entita
{
	private :
		int playerid;			// jeho id v zozname hracov
	public :
		string name;			// meno / prezyvka
		int score;				// jeho skore ?
		int zivot;				// pocet pokusov
		float vesta;			// kolk ma este vesty .. / benzinu
		bool alive;				// je na zive ?
		bool connected;			// je pripojeny ?
		int weapon;				// typ zapnutej zbrane
		DynArray <int> weapons; // vsetke zbrane co ma ....

		vec uhol;				// uhol kde pozera ..
		cPlayer();			// hrac sa pripojil
		~cPlayer();		// hrac sa odpoji

		void Think(double d);
		void Debug();
		void Touch(Entita *ent);
		virtual void DMG(Entita *ent, float dmg, DMGT typ);
		virtual void Kill();
		virtual void Spawn();
		virtual void RoundStart();
		virtual void RoundEnd();
		virtual void Connect();
		virtual void Precache();
		virtual void Disconnect();

};
#pragma endregion

extern DynArray <cPlayer> gPlayers;

bool is_player(Entita *ent);
bool is_player(int ent);

class cHuman : cPlayer {

};

class cBot : cPlayer {

};

class WeaponBox : Entita {


};


#endif;