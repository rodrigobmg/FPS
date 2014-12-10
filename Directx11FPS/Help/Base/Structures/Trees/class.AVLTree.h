		/*
Strom* PrehladavanieS( bool (*p)() );
		Strom* PrehladavanieH( bool (*p)() ); // true ak break
		void RunToAllS( bool (*p)() ); // true ak break
		void RunToAllH( bool (*p)() ); // true ak break
		*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Zaneprazdneny minister implementation
	@zadanie: 2
	@autor: Lukas Sekerak
*/
#define ALLOC (List*) malloc(sizeof(List))
#define LEFT 0
#define RIGHT 1
#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b) ((a < b) ? a : b)
#define VYSKA(a) ((!a) ? 0 : a->vyska)
#define BF(a) VYSKA(a->child[LEFT]) - VYSKA(a->child[RIGHT])
#define DATATYPE int

/*+ Hlavna struktura
	- Predstavuje list stromu
	- Obsahuje udaje o stretnuti OD-DO a ID stretnutia
	- Odkazi na deti
*/

struct _List {
	// Cutom data
	DATATYPE a, ma; //min A
	DATATYPE b, mb; //max B
	DATATYPE id;
	
	// BVS
	DATATYPE vyska;
	struct _List *child[2];
	struct _List *parent;
};
typedef struct _List List;

// Pomocne globalne
List* gRoot;
int gResult;
List* gInsert;
DATATYPE gSearch[2];

// Prototypy funkcii - hlavicky
void seky_insert(DATATYPE a, DATATYPE b, DATATYPE id);
void seky_parser();
void seky_tester();
void seky_balance(List* x);
void seky_findinterval(DATATYPE a, DATATYPE b);
void seky_findpoint(DATATYPE x);
void seky_print(List* item, const char *temp);
void seky_delete(DATATYPE x);


// Opravuj pomocne intervali
void seky_intervalfix(List *x) {
	if(x->child[LEFT] && x->child[RIGHT]) { 
		x->ma = MIN(x->child[LEFT]->ma, x->child[RIGHT]->ma);
		x->ma = MIN(x->ma,x->a);
		x->mb = MAX(x->child[LEFT]->mb, x->child[RIGHT]->mb);
		x->mb = MAX(x->mb,x->b);
	} else if (x->child[LEFT]) {
		x->ma = MIN(x->child[LEFT]->ma, x->a);
		x->mb = MAX(x->child[LEFT]->mb, x->b);
	} else if (x->child[RIGHT]) {
		x->ma = MIN(x->child[RIGHT]->ma, x->a);
		x->mb = MAX(x->child[RIGHT]->mb, x->b);
	} else {
		x->ma = x->a;
		x->mb = x->b;
	}
}

// Sprav a nastav vysku pre vrchol
void seky_makevyska(List* vrchol) {
	int a, b;
	a = VYSKA(vrchol->child[LEFT]);
	b = VYSKA(vrchol->child[RIGHT]);
	vrchol->vyska = 1 + MAX(a, b);
}

/*+ Lava a Prava rotacia
	- Fixuje nevyvazenost stromu
	- Je potrebna pri insert a delete operaciach
*/
void seky_rotacia(List* A, int typ)
{
	List* B;
	List* swapparent;

	// Sprav rotaciu
	B = A->child[!typ];
	A->child[!typ] = B->child[typ];
	B->child[typ] = A;
	
	// Vymen parenty
	swapparent = A->parent;
	A->parent = B;
	B->parent = swapparent;

	// Nastav dieta parenta A
	if(swapparent) {
		swapparent->child[ swapparent->child[RIGHT]==A ] = B;
	} else {
		gRoot = B;
	}

	// Interval fixni
	seky_intervalfix(A);
	seky_intervalfix(B);

	// Prerob nove vysky
	seky_makevyska(A);
	seky_makevyska(B);
}
void seky_doublerotacia(List* x, int smer) {
	seky_rotacia(x->child[!smer], !smer);
	seky_rotacia(x, smer);
}

// Robi vyvazenost stromu od vrcholu po koren
void seky_balance(List* x)
{
	List* it, *lastit;
	int vyska, lastvyska, rovnakysmer;
	it = x;
	lastit = gRoot; // ak by nezbehlo ani raz

	while(it != NULL) {
		seky_makevyska(it);	
		
		// Oprava balancie
		lastvyska = vyska;
		vyska = BF(it);	
		rovnakysmer = (vyska * lastvyska > 0); // tak potom + znamienko lebo ++=+ a --=-
		if(vyska >= 2) {
			if(rovnakysmer) {
				seky_rotacia(it, RIGHT);
			} else {
				seky_doublerotacia(it, RIGHT);
			}
		} else if(vyska <= -2 ) {
			if(rovnakysmer) {
				seky_rotacia(it, LEFT);
			} else {
				seky_doublerotacia(it, LEFT);
			}
		} else {
			// bf uz nemusime
			seky_intervalfix(it);
		}

		lastit = it;
		it = it->parent;
	}

	// Oprav root
	gRoot = lastit;
}

/*+ Testovace
	- Pomocne funkcie ktore prejdu strom a skontroluju ho
	- Obsahuje rozne testy
*/
int seky_TestVyvazenosti(List* vrchol) {
	short vyska;
	vyska = BF(vrchol);
	if(vyska < 0) { vyska = -1 * vyska; }
	return ( vyska <= 1);
}
void seky_tester_r(List* x)
{
	DATATYPE vyska;
	if(!x) return;

	// Test A - testuj ci vrchol ma spravne nastavenu vysku
	vyska = x->vyska;
	seky_makevyska(x);
	if(vyska != x->vyska) {
		printf("Test A - ID %d\n", x->id);
		return;
	}

	// Test B - testuj vyvazenost
	if(!seky_TestVyvazenosti(x)) {
		printf("Test B - ID %d\n", x->id);
		return;
	}
	
	// Test C -ci prvky su spravne nastavene
	if( ( x->child[LEFT] && x->child[LEFT]->id >= x->id) || ( x->child[RIGHT] && x->child[RIGHT]->id < x->id) ) {
		printf("Test C - ID %d\n", x->id);
	}

	seky_tester_r(x->child[LEFT]);
	seky_tester_r(x->child[RIGHT]);
}
void seky_tester() {
	seky_tester_r(gRoot);
}
void seky_print_r(List* item)
{
	if(item == NULL) {
		printf("- ");
		return;
	}
	printf("%d(%d) ", item->id, item->vyska);
	seky_print_r(item->child[LEFT]);
	seky_print_r(item->child[RIGHT]);
}
void seky_print(List* item, const char *temp)
{
	printf("%s:\n", temp);
	seky_print_r(item);
	printf("\n");
}

/*+ Delete
	- Rekurzivne prehladavaj strom
	- Ak najdes dany prvok, vymaze ho
	- Zabezspecuje aj vymazanie korena stromu
	- Posielame si parenta, rodica
*/
List* seky_find(DATATYPE id) {
	// Najdi prvok
	List* dalsi = gRoot;
	while(dalsi != NULL) {
		if(dalsi->id == id) {
			return dalsi; // Nasli sme prvok ktory mame vymazat
		}
		dalsi = dalsi->child[(int)(id > dalsi->id)];
	}
	return NULL;
}
int seky_successor(List* x, List** successor)
{ 
	*successor = x->child[RIGHT];
	if(*successor == NULL) {
		*successor = x->parent;
		while(x == (*successor)->child[RIGHT]) {
			x = *successor;
			*successor = (*successor)->parent;
		}
		return 0;
	}
	
	while((*successor)->child[LEFT] != NULL) {
		*successor = (*successor)->child[LEFT];
	}
	return 1;
}
void seky_prepoj(List* dedo, List* x, int strana) 
{
	// Tak ide o koren
	if(!dedo) {
		gRoot = x->child[strana];
		gRoot->parent = NULL;
		return;
	}

	// Normalny prvok
	dedo->child[ (x == dedo->child[RIGHT]) ] = x->child[strana];
	x->child[strana]->parent = dedo;
}
void seky_prepoj2(List* dedo, List* x)
{
	// Ak ide o koren
	if(!dedo) {
		gRoot = NULL;
		return;
	}
	// Nastav hodnotu na NULL
	dedo->child[ (x == dedo->child[RIGHT]) ] = NULL;
}
List* seky_odpojzostromu(List* x)
{
	// Ak nema deti
	int deti[2];
	List* dedo;
	dedo = x->parent;

	deti[LEFT] = ( x->child[LEFT] == NULL );
	deti[RIGHT] = ( x->child[RIGHT] == NULL );
	if(deti[LEFT] && deti[RIGHT]) {
		seky_prepoj2(dedo, x);
		return dedo;
	} else if(deti[LEFT]) {
		seky_prepoj(dedo, x, RIGHT);
		return dedo;
	} else if(deti[RIGHT]) {
		seky_prepoj(dedo, x, LEFT);
		return dedo;
	}
	// else ?
	return NULL;
}
void seky_delete(int x) 
{	
	List* nahrada;
	List* vymazat;
	List* dedo;
	int vysledok;

	// Najdi prvok ktory mame vymazat
	vymazat = seky_find(x);
	if(!vymazat) return;
	
	// Pokus sa vrchol normalne vymazat
	dedo = seky_odpojzostromu(vymazat);
	if(dedo == NULL) {
		// Nepodarilo sa to - najdi nahradu
		vysledok = seky_successor(vymazat, &nahrada);
		if(nahrada == NULL) {
			// Error ?
			printf("Error\n");
		} else {
			// Nahrad vsetke data
			vymazat->a = nahrada->a;
			vymazat->b = nahrada->b;
			vymazat->id = nahrada->id;

			dedo = seky_odpojzostromu(nahrada); // nahradu odpoj zo stromu			
			// nahrada moze byt uplne na lavej strane alebo hore nad vymazat
			if(vysledok) { // bola na lavej, teda fixuj intervali od deda nahrady po vymazanie
				seky_balance(nahrada);
			} else {
				seky_balance(vymazat);
			}
			free(nahrada); // fyzicky vymaz nahradu
		}
	} else {
		// Vymazalo to takze oprav intervali a vymaz prvok
		free(vymazat);
		// nahrada je dedo a od neho to opravujeme
		seky_balance(dedo);
	}
}


/*+ Insert
	- Rekurzivne prehladavaj strom
	- Ak najdes miesto kde ho ma vlozit, tak ho vloz
	- Cestou k danemu prvku opravuj farby a rotuj listy
*/
void seky_insert_findplace_r
(
	List* otec, // otec
	int pdir // smer od otca na syna
) {	
	// Nasli sme miesto kde to mame vlozit
	if (otec->child[pdir] == NULL) {
		otec->child[pdir] = gInsert;
		gInsert->parent = otec;
		return;
	}	
	//*Funkcia vracia najdeny pointer a potom sa to nastavy
	//*Hladanie nemusi byt rekurzivne

	// Rekurzivne volaj dalej
	seky_insert_findplace_r(otec->child[pdir], (int) (gInsert->id > otec->child[pdir]->id)  );
}
void seky_insert(DATATYPE a, DATATYPE b, DATATYPE id) 
{  
	// Mozme alokovat
	gInsert = ALLOC;
	if(gInsert == NULL) {
		printf("Error ALLOC\n");
	}

	// Nastav
	gInsert->child[RIGHT] = NULL;
	gInsert->child[LEFT] = NULL;
	gInsert->a = a;
	gInsert->b = b;
	gInsert->ma = a;
	gInsert->mb = b;
	gInsert->id = id;
	gInsert->vyska = 0;
	gInsert->parent = NULL;

	// Ak ide o koren - ten je trocha specialny
	if(gRoot == NULL) {
		gRoot = gInsert;
	} else {
		// Inak je to klasicka struktura
		seky_insert_findplace_r(gRoot, (gInsert->id > gRoot->id) );
	}
	//seky_print(gRoot, "Root");
	seky_balance(gInsert);
	//seky_print(gRoot, "Vyskafix");
}

/*+ Findpoint
	- Rekurzivne prehladava strom
	- Hlada hodnoty ktore vyhovuju podmienke
*/
void seky_findpoint_r(List* item)
{
	if(item == NULL) return;
	if(gSearch[0] < item->ma || gSearch[0] > item->mb) return; // interval finta

	seky_findpoint_r(item->child[LEFT]);
	if(gSearch[0] >= item->a && gSearch[0] <= item->b) {
		gResult = 1;
		printf("%d ", item->id);
	}
	seky_findpoint_r(item->child[RIGHT]);
}
void seky_findpoint(DATATYPE x) {
	gResult = 0;
	gSearch[0] = x;
	seky_findpoint_r(gRoot);
	if(gResult == 0) {
		printf("-");
	}
	printf("\n");
}

/*+ Findinterval
	- Rekurzivne prehladava strom
	- Hlada hodnoty ktore vyhovuju podmienke
*/
int seky_prienik(DATATYPE b1, DATATYPE b2) {
	if (gSearch[0] <= b1) {
		return (b1 <= gSearch[1]);
	} else {
		return (gSearch[0] <= b2);
	}
}
void seky_findinterval_r(List* item)
{
	if(item == NULL) return;
	if(!seky_prienik(item->ma, item->mb)) return; // interval finta
	
	seky_findinterval_r(item->child[LEFT]);
	if(seky_prienik(item->a, item->b)) {
		gResult = 1;
		printf("%d ", item->id);
	}
	seky_findinterval_r(item->child[RIGHT]);
}
void seky_findinterval(DATATYPE a, DATATYPE b) {
	gResult = 0;
	gSearch[0] = a;
	gSearch[1] = b;

	seky_findinterval_r(gRoot);
	if(gResult == 0) {
		printf("-");
	}
	printf("\n");
}

/*+ Parser
	- Nacitava prikazi a deli ich
	- Ziskane cisla posiela do konkretnych funkcii
*/
void seky_parser()
{
	char prikaz[100];
	int p[3];
	FILE *f;
	//f = stdin;
	f = fopen("subor.txt", "r");

	while(1) {
		fscanf(f, "%s", prikaz);
		if( prikaz == NULL || feof(f) || prikaz[0] == 'Q') {
			// Fix bug ked sa posledny prikaz spusti 2x
			return;
		}

		if(prikaz[0] == 'I') {							// INSERT
			fscanf(f, "%d", &p[0]);
			fscanf(f, "%d", &p[1]);
			fscanf(f, "%d", &p[2]);
			seky_insert(p[0], p[1], p[2]);

		} else if(prikaz[0] == 'F' && prikaz[4] == 'I') { // FINDINTERVAL
			fscanf(f, "%d", &p[0]);
			fscanf(f, "%d", &p[1]);
			fscanf(f, "%d", &p[2]);
			seky_findinterval(p[0], p[1]);

		} else if(prikaz[0] == 'F' && prikaz[4] == 'P') { // FINDPOINT
			fscanf(f, "%d", &p[0]);
			seky_findpoint(p[0]);

		} else if(prikaz[0] == 'D') {					// DELETE
			fscanf(f, "%d", &p[0]);
			seky_delete(p[0]);

		} else {
			printf("Error");
		}
	}
}
void seky_free(List* x)
{
    if(x == NULL) return;
	seky_free(x->child[LEFT]);
    seky_free(x->child[RIGHT]);
    free(x);
}
/*+ Hlavny program
	- Priprav koren
	- Spusti parser
*/
int main()
{
	clock_t timer;
	timer = clock();

	gRoot = 0;
	gInsert = 0;
	seky_parser();
	//seky_tester();

	timer = clock() - timer;
	printf("%f sec", (float) timer / (float) CLOCKS_PER_SEC);
	return 0;
}