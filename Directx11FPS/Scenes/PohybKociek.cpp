#include "PohybKociek.h"
#include "./Direct.h"
#include "./classes/Help/Algebra/class.Math.h"
#include "./classes/Entity/Base/class.Ent.h"
#include "./classes/Render/class.RenderManager.h"

void DrawLine(vec originA, vec originB, int argb = D3DCOLOR_ARGB(255, 255, 0, 0));
void DrawSustavu(vec origin, float size);
void HudText(float x, float y, string txt, D3DCOLOR color = D3DCOLOR_XRGB(255,255,255), int size = 14, float MaxSizeX = 800.0);
void DrawKvaderAA(vec origin, float apol, float bpol, float cpol, int rgba = D3DCOLOR_ARGB(255, 255, 0, 0));
void NajdiBod(vec origin);

PLUGIN PohybKociek::Init() 
{
	PLUGIN_MSG("Testovacia entita vytvorena.");
	int ent = 1;
	gEntity[ent].construct.origin.set(0.0, 0.0, 0.0);
	gEntity[ent].construct.min.set(-50.0f, -50.0f, 50.0f);
	gEntity[ent].construct.max.set(50.0f, 50.0f, -50.0f);
	gEntity[ent].gravity = 1.0f;
	gEntity[ent].mass = 1.0f;
	gEntity[ent].velocity.set(3.0f, 0.0f, 0.0f);
	gEntity[ent].accelaration = 1.0f; 
	gEntity[ent].solid = SOLID_ALL;
	//gEntity[ent].hmotnost = 100.0f;

	/* 1.0 konstantne , menej spomaluje, viacej zrychluje ...
		ak do minusu tak rychlost zmeni smer... a opet zrychluje a spomaluje
	*/

	PLUGIN_MSG("2. Testovacia entita vytvorena.");
	ent = 2;
	gEntity[ent].construct.origin.set(0.0, 0.0, 0.0);
	gEntity[ent].construct.min.set(-100.0f, -100.0f, 100.0f);
	gEntity[ent].construct.max.set(100.0f, 100.0f, -100.0f);
	gEntity[ent].gravity = 1.0f;
	gEntity[ent].mass = 1.1f;
	gEntity[ent].velocity.set(1.0f, 0.0f, 0.0f);
	gEntity[ent].accelaration = 1.0f; 
	gEntity[ent].solid = SOLID_ALL;
	//gEntity[ent].hmotnost = 101.0f;

	PLUGIN_MSG("3. Testovacia entita vytvorena.");
	ent = 3;
	gEntity[ent].construct.origin.set(0.0, 0.0, 0.0);
	gEntity[ent].construct.min.set(-200.0f, -200.0f, 200.0f);
	gEntity[ent].construct.max.set(200.0f, 200.0f, -200.0f);
	//gEntity[ent].gravity = 1.0f;// normalny vpliv 10 m/s
	//gEntity[ent].mass = 30.0f;
	//gEntity[ent].velocity.set(1.0f, 0.0f, 0.0f);
	//gEntity[ent].accelaration = 1.0001f; 
	gEntity[ent].solid = SOLID_ALL;
	//gEntity[ent].hmotnost = 101.0f;
	return PLUGIN_CONTINUE;
}