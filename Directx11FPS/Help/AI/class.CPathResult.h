#define lol
#ifndef lol

//#ifndef H_CPathResult
//#define H_CPathResult
#include "../../Global.h"
#include "./classes/help/Base/class.DynArray.h"
#include "./classes/AI/class.CMapSquare.h"
extern class CPathResult;

class CPathResult {
public:
	DynArray<CMapSquare> krok;
	int score; // sucet skore kazdeho kroku
	
	int Krokok();
	bool ExistPath(); // Ak prvkov > 1
	void Print();

	bool operator< ( const CPathResult &b );
	bool operator<=( const CPathResult &b );
	bool operator> ( const CPathResult &b ); // Ak jedna cesta je dlhsia
	bool operator>=( const CPathResult &b );
	bool operator==( const CPathResult &b ); // Prehlada obidva aporovna vsetke prvky
	bool operator!=( const CPathResult &b );

private:
	void MakeScore();
};



#endif