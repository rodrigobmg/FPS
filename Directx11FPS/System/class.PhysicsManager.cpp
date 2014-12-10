#include "class.Managers.h"
#include "../Help/App/class.Game.h"
#include "class.World.h"

PhysicsManager::PhysicsManager() {
	gravition = vec(0.0f, -9.8f, 0.0f);
}
void PhysicsManager::Frame() {
	// Vsetke funkcie tu pojdu zaradom
	DeltaTime = gGame->GetLeftTime();
	for(UINT ent=0; ent < gEntity->GetSize(); ent++) {
		gEntity->GetAt(ent)->Think(DeltaTime);
	}

	Gravitation();	
	TestCollisions();
	FinalForce();
}
void PhysicsManager::Gravitation() {
	for(UINT ent=0; ent < gEntity->GetSize(); ent++) {
		if(gEntity->GetAt(ent)->getGravity()){			// + pofix
			gEntity->GetAt(ent)->AddForce(gravition);
		}
	}
}
void PhysicsManager::TestCollisions() {
	UINT a, b;
	Entity *temp;
	for(a=0; a < gEntity->GetSize(); a++) {
		if(gEntity->GetAt(a)->isSolid()){ // TODO toto nepouzivat ale pridavat do zoznamu povolenych
			temp = gEntity->GetAt(a);
			for(b=a+1; b < gEntity->GetSize(); b++) {
				if(gEntity->GetAt(b)->isSolid()) {
					if(
						// TODO gEntity->GetAt(b)->getShape()->Test(temp->>getShape());
						true
						) {	
						ColAnswer(temp, gEntity->GetAt(b));
					}
				}
			}

			BspCollision(temp);
		}
	}
	// TODO Pouzivame pomocny OCtre
}
void PhysicsManager::ColAnswer(Entity *a, Entity *b) {
	// Vypocitaj odpoved
	// Zavolaj tocuh na A aj B
	// Aplikuj na obidve entity
	// Nastav nove F
}
void PhysicsManager::BspCollision(Entity *a) {
	// zisti v ktorej oblasti sme a tam zisti dotyk
	// prejdi celu cast a skontroluj dotyk
	//Octree *o = a->getOctree();
	//o->TestBsp(a);

}
void PhysicsManager::FinalForce() 
{
	// DeltaTime
	// Spocitaj vysledne F
	// Nastav nove A
	// Nastav nove V
	// Nastav novy Origin
}

//
//Pøi pøípravì prostøedí simulace si musíte dávat pozor na to, jaké podmínky v tomto prostøedí panují. Prostøedí v tomto tutoriálu bude prázdný prostor èekající na zaplnìní objekty, které vytvoøíme. Nejdøíve se rozhodneme, jaké jednotky použijeme pro hmotnost, èas a délku. Rozhodl jsem se použít kilogram pro hmotnost, sekundu pro èas a metr pro délku. Takže jednotky rychlosti budou m/s a jednotky zrychlení budou m/s^2 (metr za sekundu na druhou).
//
//Abychom toto všechno využili v praxi, musíme napsat tøídu, která bude reprezentovat objekt a bude obsahovat jeho hmotnost, polohu, rychlost a sílu, která na nìho pùsobí.
//
//class Mass
//
//{
//
//public:
//
//float m;// Hmotnost
//
//Vector3D pos;// Pozice v prostoru
//
//Vector3D vel;// Rychlosti a smìr pohybu
//
//Vector3D force;// Síla pùsobící na objekt
//
//V konstruktoru inicializujeme pouze hmotnost, která se jako jediná nebude mìnit. Pozice, rychlost i pùsobící síly se urèitì mìnit budou.
//
//Mass(float m)// Konstruktor
//
//{
//
//this->m = m;
//
//}
//
//Aplikujeme silové pùsobení. Objekt mùže souèasnì ovlivòovat nìkolik zdrojù. Vektor v parametru je souèet všech sil pùsobících na objekt. Pøed jeho aplikací bychom mìli stávající sílu vynulovat. K tomu slouží druhá funkce.
//
//void applyForce(Vector3D force)
//
//{
//
//this->force += force;// Vnìjší síla je pøiètena
//
//}
//
//void init()
//
//{
//
//force.x = 0;
//
//force.y = 0;
//
//force.z = 0;
//
//}
//
//Zde je struèný seznam toho, co pøi simulaci musíme provést:
//
//   1. Vynulovat sílu - metoda init()
//   2. Vypoèítat znovu pùsobící sílu
//   3. Pøizpùsobit pohyb posunu v èase
//
//----------------------------------------------------------------------------------------------------------------------
//Pro práci s èasem použijeme Eulerovu metodu, kterou využívá vìtšina her. Existují mnohem sofistikovanìjší metody, ale tahle postaèí. Velmi jednoduše se vypoèítá rychlost a poloha pro další èasový úsek s ohledem na pùsobící sílu a uplynulý èas. Ke stávající rychlosti pøièteme její zmìnu, která je závislá na zrychlení (síla/m) a uplynulém èase (dt). V dalším kroku pøizpùsobíme polohu - opìt v závislosti na èase.
//
//void simulate(float dt)
//
//{
//
//vel += (force / m) * dt;// Zmìna rychlosti je pøiètena k aktuální rychlosti
//
//pos += vel * dt;// Zmìna polohy je pøiètena k aktuální poloze
//
//}
//
//};
//------------------------------------------------------------------------------------------------------------------------
//
//
//
//
//
//
//
//
//
//
//class Simulation
//
//{
//
//public:
//
//int numOfMasses;// Poèet objektù v zásobníku
//
//Mass** masses;// Objekty jsou uchovávány v jednorozmìrném poli ukazatelù na objekty
//
//Simulation(int numOfMasses, float m)// Konstruktor vytvoøí objekty s danou hmotností
//
//{
//
//this->numOfMasses = numOfMasses;// Inicializace poètu
//
//masses = new Mass*[numOfMasses];// Alokace dynamické pamìti pro pole ukazatelù
//
//for (int a = 0; a < numOfMasses; ++a)// Projdeme všechny ukazatele na objekty
//
//masses[a] = new Mass(m);// Vytvoøíme objekt a umístíme ho na místo v poli
//
//}
//
//~Simulation()// Smaže vytvoøené objekty
//
//{
//
//release();
//
//}
//
//virtual void release()// Uvolní dynamickou pamì
//
//{
//
//for (int a = 0; a < numOfMasses; ++a)// Smaže všechny vytvoøené objekty
//
//{
//
//delete(masses[a]);// Uvolní dynamickou pamì objektù
//
//masses[a] = NULL;// Nastaví ukazatele na NULL
//
//}
//
//delete(masses);// Uvolní dynamickou pamì ukazatelù na objekty
//
//masses = NULL;// Nastaví ukazatel na NULL
//
//}
//
//Mass* getMass(int index)// Získání objektu s urèitým indexem
//
//{
//
//if (index < 0 || index >= numOfMasses)// Pokud index není v rozsahu pole
//
//return NULL;// Vrátí NULL
//
//return masses[index];// Vrátí objekt s daným indexem
//
//}
//
//Proces simulace se skládá ze tøí krokù:
//
//   1. Init() nastaví síly na nulu
//   2. Solve() znovu aplikuje síly
//   3. Simulate(float dt) posune objekty v závislosti na èase
//
//virtual void init()// Tato metoda zavolá init() metodu každého objektu
//
//{
//
//for (int a = 0; a < numOfMasses; ++a)// Prochází objekty
//
//masses[a]->init();// Zavolání init() daného objektu
//
//}
//
//virtual void solve()
//
//{
//
//// Bez implementace, protože nechceme v základním zásobníku žádné síly
//
//// Ve vylepšených zásobnících, bude tato metoda nahrazena, aby na objekty pùsobila nìjaká síla
//
//}
//
//virtual void simulate(float dt)// Výpoèet v závislosti na èase
//
//{
//
//for (int a = 0; a < numOfMasses; ++a)// Projdeme všechny objekty
//
//masses[a]->simulate(dt);// Výpoèet nové polohy a rychlosti objektu
//
//}
//
//Všechny tyto metody jsou volány v následující funkci.
//
//virtual void operate(float dt)// Kompletní simulaèní metoda
//
//{
//
//init();// Krok 1: vynulování sil
//
//solve();// Krok 2: aplikace sil
//
//simulate(dt);// Krok 3: vypoèítání polohy a rychlosti objektù v závislosti na èase
//
//}
//
//};
//
//Nyní máme jednoduchý simulaèní engine. Je založený na matematické knihovnì. Obsahuje tøídy Mass a Simulation. Používá bìžnou Eulerovu metodu na výpoèet simulace. Teï jsme pøipraveni na vývoj aplikací. Aplikace, kterou budeme vyvíjet využívá:
//
//   1. Objekty s konstantní hmotností
//   2. Objekty v gravitaèním poli
//   3. Objekty spojené pružinou s nìjakým bodem
//
//
//
//   Ovládání simulace aplikací
//
//Pøedtím než napíšeme nìjakou simulaci, mìli bychom vìdìt, jak se tøídami zacházet. V tomto tutoriálu jsou simulaèní a aplikaèní èásti oddìleny do dvou samostatných souborù. V souboru s aplikaèní èástí je funkce Update(), která se volá opakovanì pøi každém novém framu.
//
//void Update (DWORD milliseconds)// Aktualizace pohybu
//
//{
//
//// Ošetøení vstupu z klávesnice
//
//if (g_keys->keyDown [VK_ESCAPE] == TRUE)
//
//TerminateApplication (g_window);
//
//if (g_keys->keyDown [VK_F1] == TRUE)
//
//ToggleFullscreen (g_window);
//
//if (g_keys->keyDown [VK_F2] == TRUE)
//
//slowMotionRatio = 1.0f;
//
//if (g_keys->keyDown [VK_F3] == TRUE)
//
//slowMotionRatio = 10.0f;
//
//DWORD milliseconds je èas, který uplynul od pøedchozího volání funkce. Budeme poèítat èas pøi simulacích na milisekundy. Pokud bude simulace sledovat tento èas, pùjde stejnì rychle jako v reálném èase. K provedení simulace jednoduše zavoláme funkci operate(float dt). Pøedtím než ji zavoláme musíme znát hodnotu dt. Protože ve tøídì Simulation nepoužíváme milisekundy, ale sekundy, pøevedeme promìnnou milliseconds na sekundy. Potom použijeme promìnnou slowMotionRatio, která udává, jak má být simulace zpomalená vzhledem k reálnému èasu. Touto promìnnou dìlíme dt a dostaneme nové dt. Pøidáme dt k promìnné timeElapsed, která udává kolik èasu simulace už ubìhlo (neudává tedy reálný èas).
//
//float dt = milliseconds / 1000.0f;// Pøepoèítá milisekundy na sekundy
//
//dt /= slowMotionRatio;// Dìlení dt zpomalovací promìnnou
//
//timeElapsed += dt;// Zvìtšení uplynulého èasu
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//1. Objekt s konstantní rychlostí
//
//Objekt s konstantní rychlostí nepotøebuje pùsobení externí síly. Pouze vytvoøíme objekt a nastavíme jeho rychlost na (1.0f, 0.0f, 0.0f), takže se bude pohybovat po ose x rychlostí 1 m/s. Tøídu ConstantVelocity odvodíme od tøídy Simulation.
//
//class ConstantVelocity : public Simulation
//
//{
//
//public:
//
//// Konstruktor nejdøíve použije konstruktor nadøazené tøídy, aby vytvoøil objekt o hmotnosti 1 kg
//
//ConstantVelocity() : Simulation(1, 1.0f)
//
//{
//
//masses[0]->pos = Vector3D(0.0f, 0.0f, 0.0f);// Nastavíme polohu objektu na poèátek
//
//masses[0]->vel = Vector3D(1.0f, 0.0f, 0.0f);// Nastavíme rychlost objektu na (1.0f, 0.0f, 0.0f) m/s
//
//}
//
//};
//
//
//
//
//
//Aplikace síly
//
//Pøi simulacích s konstantní rychlostí jsme nepoužili sílu pùsobící na objekt, protože víme, že pokud síla pùsobí na objekt, tak mìní jeho rychlost. Pokud chceme pohyb s promìnlivou rychlostí použijeme vnìjší sílu. Nejdøíve musíme všechny pùsobící síly seèíst, abychom dostali výslednou sílu, kterou v simulaèní fázi aplikujeme na objekt.
//
//Dejme tomu, že chcete použít na objekt sílu 1 N ve smìru x. Pak do solve() napíšete:
//
//mass->applyForce(Vector3D(1.0f, 0.0f, 0.0f));
//
//Pokud chcete navíc pøidat sílu 2 N ve smìru y, napíšete:
//
//mass->applyForce(Vector3D(1.0f, 0.0f, 0.0f));
//mass->applyForce(Vector3D(0.0f, 2.0f, 0.0f));
//
//Na objekt mùžete použít libovolné množství sil, libovolných smìrù, abyste ovlivnili pohyb. V následující èásti použijeme jednoduchou sílu.
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//MotionUnderGravitation vytvoøí objekt a nechá na nìj pùsobit sílu. Touto silou bude právì gravitace, která se vypoèítá vynásobením hmotnosti objektu a gravitaèního zrychlení:
//
//F = m * g
//
//Gravitaèní zrychlení na Zemi odpovídá 9.81 m/s^2. To znamená, že objekt pøi volném pádu zrychlí každou sekundu o 9.81 m/s dokud na nìho nepùsobí žádná jiná síla než gravitace. Mùže jí být odpor vzduchu, který pùsobí vždycky, ale to sem nepatøí.
//
//
//
//
//
//
//3. Objekt spojený pružinou s bodem
//
//V tomto pøíkladì chceme spojit objekt se statickým bodem. Pružina by mìla objekt pøitahovat k bodu upevnìní a tak zpùsobovat oscilaci objektu. V konstruktoru nastavíme bod upevnìní a pozici objektu.
//
//class MassConnectedWithSpring : public Simulation
//
//{
//
//public:
//
//float springConstant;// Èím vyšší bude tato konstanta, tím tužší bude pružina
//
//Vector3D connectionPos;// Bod ke kterému bude objekt pøipojen
//
//// Konstruktor nejdøíve použije konstruktor nadøazené tøídy, aby vytvoøil 1 objekt o hmotnosti 1kg
//
//MassConnectedWithSpring(float springConstant) : Simulation(1, 1.0f)
//
//{
//
//this->springConstant = springConstant;// Nastavení tuhosti pružiny
//
//connectionPos = Vector3D(0.0f, -5.0f, 0.0f);// Nastavení pozice upevòovacího bodu
//
//masses[0]->pos = connectionPos + Vector3D(10.0f, 0.0f, 0.0f);// Nastavení pozice objektu na 10 metrù napravo od bodu, ke kterému je uchycen
//
//masses[0]->vel = Vector3D(0.0f, 0.0f, 0.0f);// Nastavení rychlosti objektu na nulu
//
//}
//
//Rychlost objektu je nula a jeho pozice je 10 metrù napravo od úchytu, takže se bude pohybovat ze zaèátku smìrem doleva. Síla pružiny se dá zapsat jako
//
//F = -k * x
//virtual void solve()// Užití síly pružiny
//
//{
//
//// Použijeme sílu na všechny objekty (zatím máme jenom jeden, ale to se mùže do budoucna zmìnit)
//
//for (int a = 0; a < numOfMasses; ++a)
//
//{
//
//Vector3D springVector = masses[a]->pos - connectionPos;// Nalezení vektoru od pozice objektu k úchytu
//
//masses[a]->applyForce(-springVector * springConstant);// Použití síly podle uvedeného vzorce
//
//}
//
//}
//
//};
//k je tuhost pružiny a x je vzdálenost od úchytu. Záporná hodnota u k znaèí, že jde o pøitažlivou sílu. Kdyby bylo k kladné, tak by pružina objekt odpuzovala, což zcela jistì neodpovídá skuteènému chování.
//
//
//
//
//*/