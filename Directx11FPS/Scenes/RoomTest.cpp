#include "RoomTest.h"
#include "./classes/App/class.App.h"
#include "./classes/App/class.Direct.h"
#include "./classes/Help/Algebra/class.Math.h"
#include "./classes/Entity/Base/class.Ent.h"

// Definujeme si nejake konstanty
#define NUM_VERTICES		36

// Rozmery
#define WALL_X			512.0f
#define WALL_Y			64.0f
#define WALL_Z			256.0f

// Farby
#define FRONT_WALL_COLOR	0xffdead;
#define RIGHT_WALL_COLOR	0xddbc8b;
#define LEFT_WALL_COLOR		0xddbc8b;
#define BACK_WALL_COLOR		0xffdead;
#define FLOOR_COLOR			0x858585;
#define CEILING_COLOR		0x555555;
struct D3DVERTEX {float x, y, z; DWORD color;} vertices[NUM_VERTICES];

RoomTest::RoomTest() { 

}
RoomTest::~RoomTest() {

}
PLUGIN RoomTest::Frame(double d)
{	   	
	// Predna stena
	vertices[0].x = WALL_X*-3;	// vsetko ma 3 vektory
	vertices[0].y = WALL_Y*-1; 
	vertices[0].z = 0; 
	vertices[0].color = FRONT_WALL_COLOR;	// a farbu
   
	vertices[1].x = WALL_X;
	vertices[1].y = WALL_Y*-1;
	vertices[1].z = 0; 
	vertices[1].color = FRONT_WALL_COLOR;
   
	vertices[2].x = WALL_X;
	vertices[2].y = WALL_Y*2;
	vertices[2].z = 0; 
	vertices[2].color = FRONT_WALL_COLOR;

	vertices[3].x = WALL_X*-3;
	vertices[3].y = WALL_Y*-1;
	vertices[3].z = 0; 
	vertices[3].color = FRONT_WALL_COLOR;
   
	vertices[4].x = WALL_X;
	vertices[4].y = WALL_Y*2;
	vertices[4].z = 0; 
	vertices[4].color = FRONT_WALL_COLOR;
   
	vertices[5].x = WALL_X*-3;
	vertices[5].y = WALL_Y*2;
	vertices[5].z = 0; 
	vertices[5].color = FRONT_WALL_COLOR;

	// Prava stena
	vertices[6].x = WALL_X;
	vertices[6].y = WALL_Y*-1;
	vertices[6].z = 0; 
	vertices[6].color = FRONT_WALL_COLOR;
   
	vertices[7].x = WALL_X;
	vertices[7].y = WALL_Y*-1;
	vertices[7].z = WALL_Z*-5;
	vertices[7].color = RIGHT_WALL_COLOR;
   
	vertices[8].x = WALL_X;
	vertices[8].y = WALL_Y*2;
	vertices[8].z = WALL_Z*-5;
	vertices[8].color = RIGHT_WALL_COLOR;

	vertices[9].x = WALL_X;
	vertices[9].y = WALL_Y*-1;
	vertices[9].z = 0; 
	vertices[9].color = FRONT_WALL_COLOR;
   
	vertices[10].x = WALL_X;
	vertices[10].y = WALL_Y*2;
	vertices[10].z = WALL_Z*-5;
	vertices[10].color = RIGHT_WALL_COLOR;
   
	vertices[11].x = WALL_X;
	vertices[11].y = WALL_Y*2;
	vertices[11].z = 0; 
	vertices[11].color = FRONT_WALL_COLOR;
   
	// Lava stena
	vertices[12].x = WALL_X*-3;
	vertices[12].y = WALL_Y*-1;
	vertices[12].z = WALL_Z*-5;
	vertices[12].color = LEFT_WALL_COLOR;
   
	vertices[13].x = WALL_X*-3;
	vertices[13].y = WALL_Y*-1;
	vertices[13].z = 0; 
	vertices[13].color = FRONT_WALL_COLOR;
   
	vertices[14].x = WALL_X*-3;
	vertices[14].y = WALL_Y*2;
	vertices[14].z = 0; 
	vertices[14].color = FRONT_WALL_COLOR;

	vertices[15].x = WALL_X*-3;
	vertices[15].y = WALL_Y*-1;
	vertices[15].z = WALL_Z*-5;
	vertices[15].color = LEFT_WALL_COLOR;
   
	vertices[16].x = WALL_X*-3;
	vertices[16].y = WALL_Y*2;
	vertices[16].z = 0; 
	vertices[16].color = FRONT_WALL_COLOR;
   
	vertices[17].x = WALL_X*-3;
	vertices[17].y = WALL_Y*2;
	vertices[17].z = WALL_Z*-5;
	vertices[17].color = LEFT_WALL_COLOR;
   
	// Zadna stena
	vertices[18].x = WALL_X;
	vertices[18].y = WALL_Y*-1;
	vertices[18].z = WALL_Z*-5; 
	vertices[18].color = BACK_WALL_COLOR;

	vertices[19].x = WALL_X*-3;
	vertices[19].y = WALL_Y*-1; 
	vertices[19].z = WALL_Z*-5;
	vertices[19].color = BACK_WALL_COLOR;
      
	vertices[20].x = WALL_X;
	vertices[20].y = WALL_Y*2;
	vertices[20].z = WALL_Z*-5;
	vertices[20].color = BACK_WALL_COLOR;

	vertices[21].x = WALL_X;
	vertices[21].y = WALL_Y*2;
	vertices[21].z = WALL_Z*-5;
	vertices[21].color = BACK_WALL_COLOR;

	vertices[22].x = WALL_X*-3;
	vertices[22].y = WALL_Y*-1;
	vertices[22].z = WALL_Z*-5;
	vertices[22].color = BACK_WALL_COLOR;
      
	vertices[23].x = WALL_X*-3;
	vertices[23].y = WALL_Y*2;
	vertices[23].z = WALL_Z*-5;
	vertices[23].color = BACK_WALL_COLOR;

	// Podlaha
	vertices[24].x = WALL_X*-3;
	vertices[24].y = WALL_Y*-1;
	vertices[24].z = WALL_Z*-5;
	vertices[24].color = CEILING_COLOR;

	vertices[25].x = WALL_X;
	vertices[25].y = WALL_Y*-1;
	vertices[25].z = WALL_Z*-5;
	vertices[25].color = CEILING_COLOR;

	vertices[26].x = WALL_X;
	vertices[26].y = WALL_Y*-1;
	vertices[26].z = 0;
	vertices[26].color = FLOOR_COLOR;

	vertices[27].x = WALL_X*-3;
	vertices[27].y = WALL_Y*-1;
	vertices[27].z = WALL_Z*-5;
	vertices[27].color = CEILING_COLOR;

	vertices[28].x = WALL_X;
	vertices[28].y = WALL_Y*-1;
	vertices[28].z = 0;
	vertices[28].color = FLOOR_COLOR;

	vertices[29].x = WALL_X*-3;
	vertices[29].y = WALL_Y*-1;
	vertices[29].z = 0;
	vertices[29].color = FLOOR_COLOR;

	// Strop
	vertices[30].x = WALL_X*-3;
	vertices[30].y = WALL_Y*2;
	vertices[30].z = WALL_Z*-5;
	vertices[30].color = CEILING_COLOR;

	vertices[31].x = WALL_X;
	vertices[31].y = WALL_Y*2;
	vertices[31].z = 0;
	vertices[31].color = FLOOR_COLOR;

	vertices[32].x = WALL_X;
	vertices[32].y = WALL_Y*2;
	vertices[32].z = WALL_Z*-5;
	vertices[32].color = CEILING_COLOR;

	vertices[33].x = WALL_X*-3;
	vertices[33].y = WALL_Y*2;
	vertices[33].z = WALL_Z*-5;
	vertices[33].color = CEILING_COLOR;

	vertices[34].x = WALL_X*-3;
	vertices[34].y = WALL_Y*2;
	vertices[34].z = 0;
	vertices[34].color = FLOOR_COLOR;

	vertices[35].x = WALL_X;
	vertices[35].y = WALL_Y*2;
	vertices[35].z = 0;
	vertices[35].color = FLOOR_COLOR;

	LPDIRECT3DVERTEXBUFFER9 pVertexObject = NULL;
	void *pVertexBuffer = NULL;
   
	if(FAILED(G::Direct->zariadenie->CreateVertexBuffer(NUM_VERTICES*sizeof(D3DVERTEX), 0, 
		D3DFVF_XYZ|D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &pVertexObject, NULL))) {
		return PLUGIN_HANDLE;
	 } 
	if(FAILED(pVertexObject->Lock(0, NUM_VERTICES*sizeof(D3DVERTEX), &pVertexBuffer, 0))) {
		return PLUGIN_HANDLE;
	}  
	memcpy(pVertexBuffer, vertices, NUM_VERTICES*sizeof(D3DVERTEX));
	pVertexObject->Unlock();
   
	/* Cele to ideme vyrenderovat */
	G::Direct->zariadenie->SetStreamSource(0, pVertexObject, 0, sizeof(D3DVERTEX));
	G::Direct->zariadenie->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	G::Direct->zariadenie->DrawPrimitive(D3DPT_TRIANGLELIST, 0, NUM_VERTICES/3);
	pVertexObject->Release();
	return PLUGIN_CONTINUE;
}

// Novy debug system cez zaznamy napr.

// Pomocne funkcie
	// Na kontrolu kolizie
	// Prepiseme funkcie na jednoduchsie ako napr. uhol a pomocou UHLOV menime
	// Na vytvorenie entity a pridavame ID

		// Vytvorime funkcie na upravu MSG sprav 
		// Povolyme windowsu normalne ist ak je minimalizovana palikacia .......pomoocu delay
	
			// Najdeme graf. zariadenie
				// Kontrola ci existuje a je vsetko v poriadku
				// Nastavyme zariadenie
			// Najdeme klavesnicu
				// Kontrola ci existuje a je vsetko v poriadku
			// Najdeme mysku	
				// Kontrola ci existuje a je vsetko v poriadku
				
		/* Ak vsetko ok pokracujeme, ...
			// Loading funkcia
				// Nacitame potrebne veci :
					// Textury
										
			// Render scena ,ktoru volame na kazdy snimok
				 **
					BeginScene
						// Premazeme vsetko s CLEAR
						// Vytvarame objekty
						// Vypneme Z buffer
						// Nacitame Skybox
					EndScene
				**
				
			// Handle Klavesnica
				// Zachytavame klavesi
				//....moju vlastnu funkciu ako v HL ak stara klavesa je ta ista a nbola uz prazdna tak drzi klavesu
				// A robyme podla toho akcie
					// Pohyb
					// Zmena uhlov
			
			// Handle ku myske
				// Menime pohlad ......zachytavame staru poziciu mysky a novu poziciu a rozdiel je pohyb
				
			// Navrhnut povrch
			
			// Svetla ?	
			
			// Hudba ?
		*/
		
	// Funkcie na vypnutie
		// Automaticky uvolni premenne
		// aj zariadenia .....pre ebzspecnost

		
		
/*		
const STRANA = 128;//px
	
stock CreateCuba( Float:hrac[3])	// x, y, z
{
	/					z       D		C
						|	--------------
						|	|		|
						|	|	*	|
						|	--------------
						|      A                    B
			--------------------------------------------	x
						| 0
						|
						|
						|
						|								
	/
	const STRANA = 128.0;//px
	
	new Float:polovickastrany, Float:kocka[4][3]
		
	polovickastrany = (STRANA / 2.0);
	// A
	kocka[0][0] = hrac[0] - polovickastrany; // x
	kocka[0][1] = hrac[1]; // y
	kocka[0][2] = hrac[2] - polovickastrany; // z
	
	// B
	kocka[1][0] = kocka[0][0] + STRANA; // x
	kocka[1][1] = kocka[0][1]; // y
	kocka[1][2] = kocka[0][2]; // z	
	
	// C
	kocka[2][0] = kocka[1][0]; // x
	kocka[2][1] = kocka[1][1]; // y
	kocka[2][2] = kocka[1][2] + STRANA; // z	
	
	// D
	kocka[3][0] = kocka[2][0] - STRANA; // x
	kocka[3][1] = kocka[2][1]; // y
	kocka[3][2] = kocka[2][2]; // z
	
}
stock CreatePodlaziePlne()	// horne a dolne tymto vytvorime
{
}
stock CreatePodlazie( Float:BodA , Float:vyska, Float:sirka, Float:medzera)	// mame dolne 4 
{	
	/	- ak vyska minusova ideme dole .....				
						z     
						|	-------------- v
						|	|   |	    |	|y
						|	|   |	*  |	|s
						|	--------------k
						|      A      sirka	a
	/	
	
}
stock Rotacia( vektor[3], uhol)	// pripadna rotacia
{
	/
		          / ---------
		    /		   /
		/	*	/
	         -----------/			
	/
}
*/