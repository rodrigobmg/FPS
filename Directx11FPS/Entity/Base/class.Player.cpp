#include "class.Player.h"
#include "./Plugin.h"
#include "./Classes/Help/class.Config.h"
#include "../EntToPlugin.h"

DynArray <cPlayer> gPlayers;

cPlayer::cPlayer() {
	uhol = vec(0.0f, 0.0f, 0.0f);
	//name = G::Game->config.default_name;
	score = 0;
	zivot = 0;
	alive = false;
	connected = false;
	take_dmg = true;
	// Pripojil sa....hned na zaciatku ..
}
cPlayer::~cPlayer() {
	// Odpojil sa ...uplne posledne sa vola...
	weapons.RemoveAll();
}
void cPlayer::Think(double d) {
	// Kazdy frame pred fyzikou
	// Zachytavame nejake specialne vlastnosti..
}
void cPlayer::Debug() {
	// Rednerujeme model podla UHLOV a pohladu
	// + renderujeme zbran a ruky [red kamerou ako HUD
}
void cPlayer::Touch(Entita *ent) {
	// Pri dotyku s hracou a ked ma nozik rob dmg
	if(this->weapon != WEAPONS_KNIVE) return;
}
void cPlayer::Kill() {
	// Objekt zabily...
	vesta = 0.0;
	hp = 0.0;
	alive = false;
	// Dame aj druhe score na smrti ?
}
void cPlayer::Spawn() {
	// Ked sa narodil ...
	weapons.RemoveAll();
	weapons.Add(WEAPONS_KNIVE);
	alive = true;
	hp = 100.0;
	vesta = 0.0;
}
void cPlayer::RoundStart() {
	// Zacalo nove kolo
	score =0;
}
void cPlayer::RoundEnd() {
	// Koniec kola, vypocitaj score a pod.	
}
void cPlayer::Connect() {
	// Plne pripojeny do hry...
	connected = true;
}
void cPlayer::Precache() {
	// Nacitavaj modely pre hraca
}
void cPlayer::Disconnect() {
	alive = false;
	connected = false;
	// Neskor sa vola DELETE na tento objekt
}
void cPlayer::DMG(Entita *ent, float dmg, DMGT typ) 
{
	hp -= dmg;
	if(hp <= 0) {
		this->Kill();
	} else {
		// Nejaky efekt DMG
	}
}


// Dlasie podfunkcie

bool is_player(Entita *ent) { return (ent->class_type == CLASS_PLAYER); }
bool is_player(int ent) { return is_player(&gEntity[ent]); }