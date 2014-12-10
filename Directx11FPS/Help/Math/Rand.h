#pragma once
#include "Math.h"
#include "../Base/DesignPattern.h"
#include "../Base/class.DynArray.h"

namespace Math {
	class Rand 
	{
	public:
		Rand() {
			Generate();
		}
		inline void Generate() {
			srand ( (unsigned int ) time(NULL) );
		}
		template<typename TYPE>
		inline TYPE Percent() {
			return ( float ) ( (rand() % 20000 ) - 10000) / 10000 );
		}
		inline bool Percent(DWORD p) {
			return ((rand() % 100) > p);
		}
		template<typename TYPE>
		inline TYPE Interval(TYPE interval) { // <0, 9> ak  zada 10
			return (TYPE) rand() % interval;
		}
		template<typename TYPE>
		inline TYPE Interval(TYPE a, TYPE b) { // a - b
			return ((b-a)*((float)rand()/RAND_MAX))+a;
		}

		//++ RandomTable - zozbiera a ulozi nejake nahodne cisla, pri nacitani sa viac krat obmenia a neskor sa toto da pouzit
		template<typename TYPE>
		class Table : public DynArray<TYPE> {
		public:
			//void Add();
			//void Generate(UINT count); // vytvori od 0 po count x nahodnych cisel,  specailnou rychlou metodou
			//void Permutacia(); // premixuje prvky v poli 
		}



	private:
		SINGLETON(Rand)
	}
}