#ifndef H_EntitaGrenade
#define H_EntitaGrenade

#include "./Global.h"
#include "../EntConst.h"
#include "class.Ent.h"

#pragma region 
class EntitaGrenade : public Entita // dalsia specificka entita
{
	private :
		int Grenadeid;			// jeho id v zozname entit granatov
	public :
		double casovac;
		bool SpadloNaZem;

		void Think(float d);			// granat, mysli casovac sa upravuje, pripadne vybuchne	
		void Touch(int ent);	// dotklo sa niecoho/ hraca alebo na zem spadlo - > zmeni akceleraciu
		void Kill();			// vybuch
		void Spawn();			// vyhodil granat
};
#pragma endregion

extern DynArray <EntitaGrenade> gGrenades;

bool is_Grenade(Entita *ent);
bool is_Grenade(int ent);

#endif;