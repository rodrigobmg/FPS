#include "class.ParticipateSystem.h"
#include "./classes/Help/Algebra/class.Math.h"
#include "./classes/Entity/Base/class.Ent.h"
#include "./classes/Physics/Base/class.Collision.h"
#include "./Direct.h"
#include "./classes/render/base/class.Draw.h"
#include "./classes/app/class.G.h"
#include "./classes/app/class.Game.h"
#include "./classes/app/class.Modules.h"
#include "./classes/Physics/class.PhysicsManager.h"

#define PARTICIPATE_COLOR		D3DCOLOR_ARGB(255, 0, 0, 255)
#define PARTICIPATE_ARANDOM		-10.0f
#define PARTICIPATE_BRANDOM		10.0f
int PARTICIPATE_TEST = 1;	
#include "./classes/UI/Base/class.Input.h"

ParticipateBod::ParticipateBod() {
	structure.min = vec(-1.5, -1.5, 1.5);
	structure.max = vec(1.5, 1.5, -1.5);
}
/*
	0 - ziadny pohyb
	1 - gravitacia
	2 - nahodny pohyb
	3 - presny pohyb castic v objekte
	4 - presny pohyb castic v objekte + gravitacny FIX
*/

Participate::Participate() {
	gravitacia = true;
	bodsize = 0;
}
Participate::Participate(int i) {
	Set(i);
}
Participate::~Participate() {
	if(!bodsize) return;
	SAFE_DELETE_ARRAY(bod);
}
void Participate::Set(int i) {
	if(bodsize == i) return;
	if(bodsize > 0) SAFE_DELETE_ARRAY(bod);

	// Pridavame....
	bod = new ParticipateBod[i];
	bodsize = i;
	/*
	if(pocet > 0) {
		// Pridavame....
		for(int i=0; i < pocet; i++) {
			bod.Add( new ParticipateBod() );
		}
	} else if (pocet < 0) {
		if( !celkovy_pocet ) return;
		pocet = abs(pocet);
		// Uberame...
		for(int i=0; i < pocet; i++) {
			bod.Remove( Math::Random(celkovy_pocet) );
		}	
	}
	*/
}
void Participate::RandomizeInBox() {
	for(int i=0; i < bodsize; i++) {
		RandomizeItem(i);
	}
}
void Participate::RandomizeItem(int i) {
	bod[i].structure.origin.Random(&box.min, &box.max);
	bod[i].velocity.Random(PARTICIPATE_ARANDOM, PARTICIPATE_BRANDOM);
}
void Participate::Render(double time) 
{
	if( Input::PressOnlyOnce('C')) {
		for(int i=0; i < bodsize; i++) {
			bod[i].velocity *= 2.0f;
		}	
	}
	if( Input::PressOnlyOnce('V')) {
		for(int i=0; i < bodsize; i++) {
			bod[i].velocity *= 0.5f;
		}	
	}

	switch(PARTICIPATE_TEST) {
		case 0 : Test0(time); break;
		case 1 : Test1(time); break;
		case 2 : Test2(time); break;
		case 3 : Test3(time); break;
		case 4 : Test4(time); break;
	}
	//this->Physics(time);

	// Debug na bbox
	static Kocka TEMP;
	box.Set2Kocka(&TEMP);
	Draw::kocka(&TEMP, PARTICIPATE_COLOR);

	// Vykreslujeme ...
	for(int i=0; i < bodsize; i++) {
		bod[i].structure.Set2Kocka(&TEMP);
		Draw::kocka(&TEMP, PARTICIPATE_COLOR);
	}

	// Specialny efekt ?
	for(int i=0; i < 50; i++) {
		bod[Math::Random(0, bodsize)].structure.Set2Kocka(&TEMP);
		Draw::kocka(&TEMP, D3DCOLOR_ARGB(255, 255, 0, 2));
	}
}
void Participate::Physics(double DeltaTime) {
	this->Collision();
	// Gravitacia
	if(gravitacia) {
		for(int i=0; i < bodsize; i++) {
			bod[i].velocity.y -= G::Game->physics->gravity / 100.0; // len 10.0 pre test 1
		}
	}
	// Pohyb
	for(int i=0; i < bodsize; i++) {
		bod[i].structure.origin += bod[i].velocity / DeltaTime / 100000.0f;
		/*bod[i].structure.origin.x += bod[i].velocity.x / DeltaTime / 100000.0f; // + 00 pre TEST 1
		bod[i].structure.origin.y += bod[i].velocity.y / DeltaTime / 100000.0f;
		bod[i].structure.origin.z += bod[i].velocity.z / DeltaTime / 100000.0f;*/
		bod[i].structure.Resize();
	}
}
void Participate::Collision() {
	for(int i=0; i < bodsize; i++) {
		if( !box.Test(&bod[i].structure.origin) ) {
			// Neskor podla UHLA na stranu sa otoci...
			// Ak stena je "dolna"/zem tak pripocitaj k Y trocha aby gravitacia sa dalej nesirila...
			bod[i].velocity *= -1; // otocime vektor ....
		}
	}
}


/*
	0 - ziadny pohyb
	1 - gravitacia
	2 - nahodny pohyb
	3 - presny pohyb castic v objekte
	4 - presny pohyb castic v objekte + gravitacny FIX
*/

void Participate::Test0(double DeltaTime) {

}
void Participate::Test1(double DeltaTime) {
	int i;
	const double k = -0.9999;
	for( i=0; i < bodsize; i++) {
		if(!box.Test(&bod[i].structure.origin) ) {
			bod[i].velocity *= k; // otocime vektor ....
		} else {
			bod[i].velocity.y -= 10.0f / 10.0f;
		}
		bod[i].structure.origin.y += bod[i].velocity.y / DeltaTime / 1000000.0f;
		bod[i].structure.Resize();
	}
}
void Participate::Test2(double DeltaTime) {
	// Gravitacia
	int i;
	for(i=0; i < bodsize; i++) {
		bod[i].velocity.y -= G::Game->physics->gravity / 100.0;
	}
	for(i=0; i < bodsize; i++) {
		if( !box.Test(&bod[i].structure.origin) ) {
			bod[i].velocity *= -1; // otocime vektor ....
		}
	}
	// Pohyb
	for(i=0; i < bodsize; i++) {
		bod[i].structure.origin.x += bod[i].velocity.x / DeltaTime / 1000000.0f;
		bod[i].structure.origin.y += bod[i].velocity.y / DeltaTime / 1000000.0f;
		bod[i].structure.origin.z += bod[i].velocity.z / DeltaTime / 1000000.0f;
	}
}
void Participate::Test3(double DeltaTime) {
	// Prechadzame cez subory...
	int i;

	for(i=0; i < bodsize; i++) {
		// Gravitacia
		bod[i].velocity.y -= G::Game->physics->gravity / 100.0;

		// Collision
		switch( Collision::is_Special8Hran(&bod[i].structure, &box) ) 
		{
			case COLL_TOUCH : {
				for(int a=0; a < bodsize; a++) {
					if( COLL_IN == 
						Collision::is_Special8Hran(&bod[i].structure, &bod[a].structure) ) {
					} {
						// Tiez sa otoci ??
						bod[i].velocity *= -1;
						break; // a aj break ?
					}
				}
				break;
			}
			default : { // je uz mimo
				bod[i].velocity *= -1; // otocime vektor ....
				break;
			}
		}
		// Pohyb
		bod[i].structure.origin.x += bod[i].velocity.x / DeltaTime / 1000000.0f;
		bod[i].structure.origin.y += bod[i].velocity.y / DeltaTime / 1000000.0f;
		bod[i].structure.origin.z += bod[i].velocity.z / DeltaTime / 1000000.0f;
	}
}
void Participate::Test4(double DeltaTime) {

}