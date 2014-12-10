#define lol
#ifndef lol

#include "class.CPathResult.h"
#include "./classes/render/base/class.Draw.h"

int CPathResult::Krokok() {
	return krok.GetSize();
}
bool CPathResult::ExistPath() {
	return ( this->krok.GetSize() > 1);
}
void CPathResult::MakeScore() {
	score = 0;
	for(int i=0; i < krok.GetSize(); i++) {
		score += krok[i].score[2];
	}
}




// Operatory :)
bool CPathResult::operator< ( const CPathResult &b ) {
	return ( krok.GetSize() < b.krok.GetSize());
}
bool CPathResult::operator<= ( const CPathResult &b ) {
	return ( krok.GetSize() <= b.krok.GetSize());
}
bool CPathResult::operator> ( const CPathResult &b ) {
	return ( krok.GetSize() > b.krok.GetSize());
}
bool CPathResult::operator>= ( const CPathResult &b ) {
	return ( krok.GetSize() >= b.krok.GetSize());
}
bool CPathResult::operator== ( const CPathResult &b ) {
	if(krok.GetSize() != b.krok.GetSize()) return false;
	// Teraz skontroluj jednotlive kroky
	for(int i=0; i < krok.GetSize(); i++) {
		if(krok[i].id[2] != b.krok[i].id[2]) return false; // Skoda keby boli pointre :(...
	}
	return true;
}
bool CPathResult::operator!= ( const CPathResult &b ) {
	if(krok.GetSize() != b.krok.GetSize()) return true;
	// Teraz skontroluj jednotlive kroky
	for(int i=0; i < krok.GetSize(); i++) {
		if(krok[i].id[2] != b.krok[i].id[2]) return true; // Skoda keby boli pointre :(...
	}
	return false;
}
void CPathResult::Print() 
{
	// Cesta
	vec * a, * b;
	int i;

	for(i=0; i < krok.GetSize()-1; i++) {
		a = &krok[i].box.origin;
		b = &krok[i].box.origin;
		Draw::Line(a, b, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
}
#endif