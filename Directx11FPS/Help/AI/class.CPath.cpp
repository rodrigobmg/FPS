#define lol
#ifndef lol

#include "class.CPath.h"
#include "./classes/render/base/class.Draw.h"

/*
CPath * START = new CPath();
CPath * END = new CPath();
START->box.origin.x = 100;
START->box.origin.y = 100;
START->size = 10.0f;
START->DojdiK(END);
*/



CPath::CPath() {
	Default();
}
CPath::~CPath() {
	openlist.RemoveAll();
	SAFE_DELETE(calling);
	paths.RemoveAll();
	SAFE_DELETE(closelist);
	SAFE_DELETE(end);
	SAFE_DELETE(called_from);
}
void CPath::Default() {
	// Defaultne
	bad = false;
	for(int i=0; i < 3; i++) {
		score[i] = -1;
		id[i] = -1;
	}
	openlist.SetSize(8);
	binaryat = 1;
}
void CPath::DojdiK(CPath * a) {
	end = a;
	if(ProblematicSituations()) return;
	HladajDalsie();
}
void CPath::HladajDalsie() 
{
	cyklov++;
	GetParents();
	//SetParents();
	PrefilterCollision();
	if(called_from) PrefilterParentOpenList();
	
	// Open list je prazdny
	if(!openlist.GetSize()) {
		if(called_from != NULL) {
			called_from->HladajDalsie();
		} 
		return;
	}
	
	// Cesta pokracuje dalej
	if( binaryat >= closelist->heaps.GetSize()) {
		// Aj tu je cesta na konci, volaj rodica inak nic
		if(called_from != NULL) {
			called_from->HladajDalsie();
		} 
		return;
	}
	Ohodnot();
	SortPaths();
	CPath * dalsiepolicko;
	dalsiepolicko = closelist->heaps.GetAt(binaryat).id;
	int dalsieskore = dalsiepolicko->score[2];

	// Dlasie skore je vacsie ako to nase
	if(dalsieskore > score[2]) {
		if(called_from != NULL) {
			// SKus sa vratit na rodia a najst dalsie
			called_from->HladajDalsie();
			return;
		}
	}

	// Co ak sme uz na konci ?
	if(dalsiepolicko == end) {
		return;
	}

	binaryat++;
	calling = dalsiepolicko;
	dalsiepolicko->HladajDalsie();
}
CPathResult CPath::Make() {
	// Pridaj seba...
	CPathResult result;
	CPath * i;

	// Ak ma deti
	i = this;
	while(true) {
		result.krok.Add(*i);
		if(i->calling == NULL) break;
		i = i->calling;
	}
	return result;
}








	// Pomocne

void CPath::SortPaths() {


}
bool CPath::ProblematicSituations() {
	if(bad || end->bad) return true;
	if(box.origin == end->box.origin) return true;
	return false;
}
void CPath::PrefilterCollision() 
{
	int idopen, i , b;	
	for(i=0; i < openlist.GetSize(); i++) {
		// Je to uz v open liste parenta ?
		idopen = openlist[i]->id;
		for(b=0; b < called_from->openlist.GetSize(); b++) {
			if( called_from->openlist[b] == id) openlist[i].Remove(b); 
			//parent->openlist[b] = -1; // Neodstranuje sa - len sa napise -1
		}

		// Alebo je to samotny parent ?
		if(idopen == called_from->id[2]) openlist[i].Remove(b); 
	}
}/*
int CMapZone::PrefilterCollision(CPath * a) {
	// Alebo jednoducho len kontrolovat, ci dany box je zly
	CMapSquare *cell;
	for(int i=0; i < blocks.GetSize(); i++) {
		cell = &zones.GetAt( blocks.GetAt(i) );
		if(cell->id[0] == a->id[0] && cell->id[1] == a->id[1]) return i;
	}
	return -1;
}*/
void CPath::Ohodnot() {
	for(int i=0; i < a->zoznam.GetSize(); i++) {
		SetScore(&zones[a->zoznam.GetAt(i)]);
	}
}
void CPath::Ohodnot(CPath *a) {
	// F = G + H
	vec temp;
	// Sa nastavuje podla daneho typu
	temp = zones[start].box.origin - i->box.origin;
	i->score[0] = temp.Length();
	temp = zones[end].box.origin - i->box.origin;
	i->score[1] = temp.Length() * 10.0f;
	i->score[2] = i->score[0] + i->score[1];
}
void CPath::GetParents() {
	// Zaciname podla hodiniek, od 12 hod
	/*
		1	2	3
		8	*	4
		7	6	5
	*/
	vec bod = zones.GetAt(i).box.origin; 
	vec temp;
	
	temp = vec(0.0, MS_SIZE, 0.0) + bod;
	a->Add( AddBox(&temp) );

	temp = vec(MS_SIZE, MS_SIZE, 0.0) + bod;
	a->Add( AddBox(&temp ) ); // Zoznam ADD nepridava hodnoty, preco ? -> Alokacia pamete

	temp = vec(MS_SIZE, 0.0, 0.0) + bod;
	a->Add( AddBox(&temp) );

	temp = vec(MS_SIZE, -MS_SIZE, 0.0) + bod;
	a->Add( AddBox(&temp) );

	temp = vec(0.0, -MS_SIZE, 0.0) + bod;
	a->Add( AddBox(&temp) );
	
	temp = vec(-MS_SIZE, -MS_SIZE, 0.0) + bod;
	a->Add( AddBox(&temp) );
	
	temp = vec(-MS_SIZE, 0.0, 0.0) + bod;
	a->Add( AddBox(&temp) );

	temp = vec(-MS_SIZE, MS_SIZE, 0.0) + bod;
	a->Add( AddBox(&temp) );

	SetParent(&zones.GetAt(i), a);
	//for(int i=0; i < a->GetSize(); i++) printf("%d \n", a->GetAt(i));
	//printf("\n");
}
void CPath::MakeID() {
	// ID je podla suradnic - neskor implementovat nejaky algormitnus ktory spravy 1 id
	id[0] = box.origin.x / size;
	id[1] = box.origin.y / size;
}
void CPath::Print() {
	Kocka a;
	
	// Open list
	for(int i=0; i < openlist.GetSize(); i++) {
		if(openlist[i] != NULL) {
			openlist[i]->box.Set2Kocka(&a);
			Draw::kocka(&a, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	// Dalsie deti
	if(calling != NULL) calling->Print();
}
#endif

