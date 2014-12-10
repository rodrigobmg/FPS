#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* Zadanie c.3 - Nakupovanie
	Autor: Lukas Sekerak
*/
#define UINT unsigned int
#define MAX_INT 65535
#define OZNACENY '+'
#define NEOZNACENY '-'

struct _Sused 
{	// Definuje suseda vrcholu - cez hranu
	UINT id;
	UINT dlzka;

	struct _Sused* next;
};
struct _DInfo 
{	// Zdruzene informacie potrebne pre Dijkstru 
	UINT dlzka;			// Vzdialenost od pociatocneho vrcholu
	char status;		// 1 tento vrchol je oznaceny, 0 nieje
	UINT prev;			// Predchodca
};
typedef struct _DInfo DInfo;
typedef struct _Sused Sused;

UINT gVrcholov;			// Pocet vrcholov v grafe
UINT gHran;				// Celkovy pocet hran v grafe

Sused** gVrchol;		// Zoznam susedov pre kazdy vrchol.
Sused*** gVrcholLast;	// Pointer na posledneho suseda pre dany vrchol 100 000
UINT* gShop;			// Zoznam obchodov
UINT gShops;			// Pocet obchodov
DInfo* gInfo;			// Informacie o dlzke_od_startu a predchodcu pre kazdy vrchol
UINT gStart;			// Startovaci vrchol

UINT* gQueue;			// Min prioritny rad reprezentovany polom, obsahom su IDcka vrcholov
UINT* gQLink;			// Kazdy vrchol odkazuje na ID v min-prioritnom rade, obsahom su IDcka na queue
UINT gQueuePozicia;		// Pozicia v queue - prvky nemaze ale posuva ukazovatel

void hrana_add(UINT x, UINT y, UINT dlzka);

void graf_load() 
{
	FILE* vstup;
	UINT i, x, y, d;
	
	// Odkial budeme citat ?
	gVrcholov = gHran = 0;
	vstup = fopen("subor.txt", "r"); // vstup2
	fscanf(vstup, "%d %d", &gVrcholov, &gHran);
	
	// Vrcholy priprav pamet
	gVrchol = calloc(gVrcholov, sizeof(Sused)); // Priprav pamet 100 000 * ( sizeof(Sused)+sizeof(Sused*) )
	if(gVrchol == NULL) printf("Error calloc gVrchol\n");
	gVrcholLast = calloc(gVrcholov, sizeof(Sused*));
	if(gVrcholLast == NULL) printf("Error calloc gVrcholLast\n");

	// Hrany nacitaj
	for(i=0; i < gHran; i++) {
		// Nacitavame a ukladame do SLL
		fscanf(vstup, "%d %d %d", &x, &y, &d);
		if(x == 1 && y == 3 && d == 875) continue;
		hrana_add(x, y, d);
	}

	// Nacitaj obchody
	fscanf(vstup, "%d", &gShops);
	gShops++;
	gShop = calloc(gShops, sizeof(UINT));
	gShop[0] = 0; // Manualne si 0 pridame medzi obchody
	for(i=1; i < gShops; i++) fscanf(vstup, "%d", &gShop[i]);
}
void hrana_free(Sused *hrana) {
	if(hrana != NULL) hrana_free(hrana->next);
	free(hrana);
}
void graf_free() {
	// Najprv pomocne
	UINT i;
	free(gVrcholLast);

	// Teraz kazdu hranu
	for(i=0; i < gVrcholov; i++) hrana_free(gVrchol[i]);
}

// Pridaj hranu do zoznamu susedov
void hrana_add(UINT x, UINT y, UINT dlzka) {
	UINT c, d;
	if(x > y) { 
		c = y;
		d = x;
	} else {
		c = x;
		d = y;
	}
	/* VYLEPSENIE - Uloz na mensiu poziciu.
		Algoritmus vyhladavania potom bude rychlejsi.
		Dostaneme nieco take v zozname susedov :
		|||||||||||||||||||||||||||||||
		||||||||||||||||||||||||
		|||||||||||||||||
		||||||||||
		|||||
		||
		|
	*/
	// Ak v zozname pre dany vrchol este nemame ziadnu hranu, nastav pointer
	if(gVrchol[c] == NULL) gVrcholLast[c] = &gVrchol[c];

	// Pridavame na koniec zoznamu
	*gVrcholLast[c] = malloc(sizeof(Sused));
	if(*gVrcholLast[c] == NULL) printf("Error malloc *gVrcholLast[c]\n");
	(*gVrcholLast[c])->id = d;
	(*gVrcholLast[c])->dlzka = dlzka;
	(*gVrcholLast[c])->next = NULL;
	gVrcholLast[c] = &((*gVrcholLast[c])->next);
}
Sused* hrana_obsahuje(UINT x, UINT y) 
{	// Funkcia nam hlada vrchol v zozname hran pre dany vrchol
	Sused* temp;
	temp = gVrchol[x];
	while(temp != NULL) {
		/* VYLEPSENIE
			Hrany by mali byt usporiadne v strome nie v SLL.
			Tejto funkcie by to pomohlo.
			z N -> log N
		*/
		if(temp->id == y) return temp;
		temp = temp->next;
	}
	return temp;
}
void hrana_print(UINT x) {
	Sused* temp;
	temp = gVrchol[x];
	printf("%d: ", x);
	while(temp != NULL) {
		printf("%d %d -> ", temp->id, temp->dlzka);
		temp = temp->next;
	}
	printf("\n");
}
void hrana_prints() {
	UINT i;
	for(i=0; i < gVrcholov; i++) hrana_print(i);
}
void info_prints() {
	UINT i;
	for(i=0; i < gVrcholov; i++)  printf("%d. %c %d %d\n", i, gInfo[i].status, gInfo[i].dlzka, gInfo[i].prev);
}
void shop_prints() {
	UINT i;
	for(i=0; i < gShops; i++) printf("%d ", gShop[i]);
	printf("\n");
}



void queue_create() {
	// Vytvor min-prioritny rad
	gQueue = malloc(gVrcholov * sizeof(UINT));
	if(gQueue == NULL) printf("Error malloc gQueue\n");
	gQLink = malloc(gVrcholov * sizeof(UINT));
	if(gQLink == NULL) printf("Error malloc gQLink\n");
	gQueuePozicia = 0;
}
#define queue_isntempty() gQueuePozicia != gVrcholov
#define queue_delete() free(gQueue); free(gQLink);
#define queue_minimum() gQueue[gQueuePozicia];
// Vymaz minimum cize prvom na 0 pozicii
// Improvizujeme, posuvame ukazovatel
#define queue_remove() gQueuePozicia++;
// Ked pozname ID vrcholu ale nie ID v queue
#define queue_refactor2(VRCHOL) queue_refactor(gQLink[VRCHOL])


void queue_swap(UINT a, UINT b) {
	// Vymen 2 prvky
	UINT temp;
	temp = gQueue[a];
	gQueue[a] = gQueue[b];
	gQueue[b] = temp;

	// Musime opravit aj odkazi
	gQLink[gQueue[a]] = a;
	gQLink[gQueue[b]] = b;
}
void queue_refactor(UINT id) {
	// Udaj na pozicii ID sa zmensil, posuva sa len do lava
	// VYLEPSENIE - Halda by bola efektivnejsia,...
	int i;
	UINT dlzka = gInfo[ gQueue[id] ].dlzka;

	// Posuvaj sa do lava od indexu
	i=id-1; // Staci nam kontrolovat index pred nim
	while(i >= 0 && (gInfo[ gQueue[i] ].dlzka > dlzka)) { i--; } // !!! swapovat treba aj postupne
	i++;

	// Ma zmysel to vymenit ?
	if(id != i) queue_swap(i, id);

	// Ak i kleslo za vrcholy ktore uz mame oznacene nastala chyba !
	if(i < gQueuePozicia ) {
		printf("Error queue_refactor\n");
	}
}
void queue_prints() {
	UINT i;
	printf("# Q  D  L\n");
	for(i=gQueuePozicia; i < gVrcholov; i++) {
		// 2 a 3 maju byt rovnake
		printf("%d. %d %d %d\n", i, gQueue[i], gInfo[ gQueue[i] ].dlzka, gQLink[i]);
	}
}



void dijkstra_relax(UINT A, UINT novysused, UINT hrana) {
	// Zakladne relaxacia pre dijkstru
	UINT hranadlzka = hrana + gInfo[A].dlzka;
	if (hranadlzka < gInfo[novysused].dlzka) {
		gInfo[novysused].dlzka = hranadlzka;
		gInfo[novysused].prev = A;
		queue_refactor2(novysused);
	}
}
void dijkstra_init(UINT start) {
	UINT i;

	// Vytvor siet informacii pre kazdy vrchol
	gInfo = malloc(gVrcholov * sizeof(DInfo));
	if(gInfo == NULL) printf("Error malloc gInfo\n");
	queue_create();

	for(i=0; i < gVrcholov; i++) {
		gInfo[i].dlzka = MAX_INT;
		gInfo[i].prev = MAX_INT;
		gInfo[i].status = NEOZNACENY;

		// Rad dosad
		gQueue[i] = i;
		gQLink[i] = i;
	}

	// Nastav zaciatok a pociatocne hodnoty
	gStart = start;
	gInfo[start].dlzka = 0;
	queue_refactor(start);
}
void dijkstra_relaxujsusedov(UINT najmensi) {	
	// Relaxuj na kazdeho suseda
	Sused* temp;
	UINT i;

	// Mensie vrcholy mozu obsahovat najmensi
	for(i=0; i < najmensi; i++) {
		if(gInfo[i].status == OZNACENY) continue; 
		temp = hrana_obsahuje(i, najmensi);
		// 17 27 37 47 57 ....
		// Vieme ze v zozname A hran nieje dvojica rovnakych vrcholov, teda AB AB
		// Teda vzdy hladame len raz
		// !!! temp ci je oznaceny
		if(temp != NULL) {
			dijkstra_relax(najmensi, i, temp->dlzka);
		}
	}

	// Spracuj samotny zoznam susedov vrcholu najmensi
	temp = gVrchol[najmensi];
	while(temp != NULL) {
		if(gInfo[temp->id].status == NEOZNACENY) {
			dijkstra_relax(najmensi, temp->id, temp->dlzka);
		}
		temp = temp->next;
	}

	// VYLEPSENIE 
	// Vdaka usporiadaniu vieme ze vyssie vrcholy su urcite vo vrchole najmensi
	// A nemusime tak prehladavat druhu polovicu vrcholov za vrcholom najmensi
}
void dijkstra() {
	UINT najmensi;
	while( queue_isntempty() ) {
		// Vyber ten najmensi vrchol
		najmensi = queue_minimum(); 
		queue_remove();
		gInfo[najmensi].status = OZNACENY;

		// Relaxuj susedov
		dijkstra_relaxujsusedov(najmensi);
	}
}
void dijkstra_printpath(UINT temp)
{
	printf("Najkratsia cesta do %d je: %d | ", temp, gInfo[temp].dlzka);
	while(temp != gStart) {
		printf("%d ", gInfo[temp].dlzka);
		temp = gInfo[temp].prev;
	}
	printf("\n");
}

UINT** gMatica;
UINT gMaticaVyska, gMaticaSirka;
UINT gPermMin;
UINT* gPermutacie;
UINT matica_get(UINT a, UINT b);

#define dijkstra_free() free(gInfo); queue_delete()
#define Permutacie_free() free(gPermutacie)

void Permutacie_prints() {
	UINT i;
	for(i=0; i < gShops; i++) printf("%d ", gPermutacie[i]);
	printf("\n");
}
void Permutacie_init() {
	gPermutacie = malloc(gShops * sizeof(UINT));
	gPermMin = MAX_INT;
	gPermutacie[0] = 0;
}
int PermutacieObsahuju(UINT hladaj, UINT maxindex) {
	UINT b; // 0 index nemusime kontrolovat a hladame po nejake maximum
	for(b=1; b < maxindex; b++) { if(gPermutacie[b] == hladaj) return 1; }
	return 0; // nenasli sme
}
void Permutacie(UINT level, UINT dlzka) {
	UINT a, novavzdialenost;

	// Prisli sme na uroven kde nam urcite neostanu dalsie moznosti
	if(level == gShops) {
		// Takze co ked nova dlzka je mensia ako doterajsie minimum 
		//Permutacie_prints();
		dlzka += matica_get(0, gPermutacie[level-1]); // Obchodni ksa ma vratit do povodneho vrcholu
		//printf("%d\n" , dlzka);
		if(dlzka < gPermMin) gPermMin = dlzka;

		// Program sa po rekurzii vrati spat na hornu cast stromu
		return;
	}

	//  Hladaj dalej
	for(a=1; a < gShops; a++)  // Vzdy zacinam od 1, lebo gShop[0] je 0 stale
	{
		// Ak na doterajsich hodnotach sme uz taku moznost mali
		if(PermutacieObsahuju(a, level)) continue; // tak ju preskoc
		
		// Level urcuje index na ktory ulozime dalsiu moznost
		gPermutacie[level] = a;
		
		// Zistime posledny index a aktualny index, vypocitame vzdialenost pre ne
		novavzdialenost = matica_get(gPermutacie[level-1], a) + dlzka;

		// Ked tato nova vzdialenost je uz teraz vacsia ako nase minimum tak nemusime ist do podstromu
		if(novavzdialenost >= gPermMin) continue;

		// Stromovo volaj dalej ...
		Permutacie(level + 1, novavzdialenost);
	}
}
void Permutacie_zero() {
	// Specialne permutacie pre prvky od korena, rychlejsie to ide
	// Kedze cast funkcii mozme vynechat
	UINT a, novavzdialenost;

	//  Hladaj dalej
	for(a=1; a < gShops; a++) {		
		gPermutacie[1] = a;
		novavzdialenost = matica_get(0, a);
		if(novavzdialenost >= gPermMin) continue;
		Permutacie(2, novavzdialenost);
	}
}
void matica_prints() {
	UINT a, b;
	for(b=0; b < gMaticaVyska; b++) {
		for(a=0; a < gMaticaSirka; a++) printf("%d ", gMatica[a][b]);
		printf("\n");
	}
}
UINT matica_get(UINT a, UINT b) {
	// A sa nesmie rovnat B, lebo krizom su 0
	if( a == b) printf("error matica_get\n");

	// A musi byt mensie ako B
	// VYLEPSENIE Vdaka tomu nemusime kopirovat hodnoty na pravu aj lavu stranu matice
	// VYLEPSENIE if(gMatica[stlpec] == NULL) matica_addcolumn(stlpec); mozme dynamicky pocas behu dotvarat maticu
	if(a > b) {
		return gMatica[b][a];
	} else {
		return gMatica[a][b];
	}
}
void matica_addcolumn(UINT stlpec) {
	// Nemame takze ho pridame
	UINT i;
	gMatica[stlpec] = calloc(gMaticaVyska, sizeof(UINT));

	// Mozno keby sme dijsktru prerobili aby hladal z A do B tak by to bolo rychlejsie
	dijkstra_init(gShop[stlpec]);
	dijkstra();

	// Vyber vysledok a uloz do matice
	for(i=stlpec+1; i < gMaticaVyska; i++) gMatica[stlpec][i] = gInfo[ gShop[i] ].dlzka;
	dijkstra_free();
}
void matica_create() {
	UINT i;
	gMaticaVyska = gShops;
	gMaticaSirka = gShops-1;
	gMatica = calloc(gMaticaSirka, sizeof(UINT*));
	
	// Vytvor maticu s udajmy
	for(i=0; i < gMaticaSirka; i++) {
		matica_addcolumn(i);
	}
}
void matica_delete() {
	UINT i;
	for(i=0; i < gMaticaSirka; i++) free(gMatica[i]);
	free(gMatica);
}
int main()
{
	clock_t timer;
	timer = clock();

	// Backtracking
	graf_load();
	matica_create();
	graf_free();
	//matica_prints();

	timer = clock() - timer;
	printf("%.15f sec\n", (double) timer / (double) CLOCKS_PER_SEC);

	//shop_prints();
	matica_prints();
	Permutacie_init();
	Permutacie_zero();
	Permutacie_free();
	printf("%d\n", gPermMin); // Obsahuju minimalnu moznost

	matica_delete();

	timer = clock() - timer;
	printf("%.15f sec\n", (double) timer / (double) CLOCKS_PER_SEC);
	return 0;
}