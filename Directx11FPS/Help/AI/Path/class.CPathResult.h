#pragma once
#include "../../Help/Base/class.DynArray.h"
#include "class.Tile.h"

namespace Path {

class Result : public DynArray<Tile*>
{
private:
	UINT m_score; // sucet skore kazdeho kroku
	
	inline void PathResult::MakeScore() {
		m_score = 0;
		for(int i=0; i < GetSize(); i++) {
			m_score += getAt(i)->score;
		}
	}


public:
	Result() {
		m_score = 0; 
	}
	inline bool PathResult::Exist() {
		return GetSize() > 1;
	}
	void PathResult::Print() {
		/* Cesta
		vec * a, * b;
		int i;

		for(i=0; i < krok.GetSize()-1; i++) {
			a = &krok[i].box.origin;
			b = &krok[i].box.origin;
			Draw::Line(a, b, D3DCOLOR_ARGB(255, 255, 0, 0));
		}*/
	}




	// Operatory :)
	inline bool PathResult::operator< ( const PathResult &b ) {
		return ( GetSize() < GetSize());
	}
	inline bool PathResult::operator<= ( const PathResult &b ) {
		return (GetSize() <= GetSize());
	}
	inline bool PathResult::operator> ( const PathResult &b ) {
		return ( krok.GetSize() > GetSize());
	}
	inline bool PathResult::operator>= ( const PathResult &b ) {
		return ( krok.GetSize() >= GetSize());
	}
	inline bool PathResult::operator== ( const PathResult &b ) {
		if(b.GetSize() != GetSize()) return false;
		// Teraz skontroluj jednotlive kroky
		for(UINT i=0; i < krok.GetSize(); i++) {
			if(getAt(i) != b.getAt(i)]) return false;
		}
		return true;
	}
};


}
