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
//P�i p��prav� prost�ed� simulace si mus�te d�vat pozor na to, jak� podm�nky v tomto prost�ed� panuj�. Prost�ed� v tomto tutori�lu bude pr�zdn� prostor �ekaj�c� na zapln�n� objekty, kter� vytvo��me. Nejd��ve se rozhodneme, jak� jednotky pou�ijeme pro hmotnost, �as a d�lku. Rozhodl jsem se pou��t kilogram pro hmotnost, sekundu pro �as a metr pro d�lku. Tak�e jednotky rychlosti budou m/s a jednotky zrychlen� budou m/s^2 (metr za sekundu na druhou).
//
//Abychom toto v�echno vyu�ili v praxi, mus�me napsat t��du, kter� bude reprezentovat objekt a bude obsahovat jeho hmotnost, polohu, rychlost a s�lu, kter� na n�ho p�sob�.
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
//Vector3D vel;// Rychlosti a sm�r pohybu
//
//Vector3D force;// S�la p�sob�c� na objekt
//
//V konstruktoru inicializujeme pouze hmotnost, kter� se jako jedin� nebude m�nit. Pozice, rychlost i p�sob�c� s�ly se ur�it� m�nit budou.
//
//Mass(float m)// Konstruktor
//
//{
//
//this->m = m;
//
//}
//
//Aplikujeme silov� p�soben�. Objekt m��e sou�asn� ovliv�ovat n�kolik zdroj�. Vektor v parametru je sou�et v�ech sil p�sob�c�ch na objekt. P�ed jeho aplikac� bychom m�li st�vaj�c� s�lu vynulovat. K tomu slou�� druh� funkce.
//
//void applyForce(Vector3D force)
//
//{
//
//this->force += force;// Vn�j�� s�la je p�i�tena
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
//Zde je stru�n� seznam toho, co p�i simulaci mus�me prov�st:
//
//   1. Vynulovat s�lu - metoda init()
//   2. Vypo��tat znovu p�sob�c� s�lu
//   3. P�izp�sobit pohyb posunu v �ase
//
//----------------------------------------------------------------------------------------------------------------------
//Pro pr�ci s �asem pou�ijeme Eulerovu metodu, kterou vyu��v� v�t�ina her. Existuj� mnohem sofistikovan�j�� metody, ale tahle posta��. Velmi jednodu�e se vypo��t� rychlost a poloha pro dal�� �asov� �sek s ohledem na p�sob�c� s�lu a uplynul� �as. Ke st�vaj�c� rychlosti p�i�teme jej� zm�nu, kter� je z�visl� na zrychlen� (s�la/m) a uplynul�m �ase (dt). V dal��m kroku p�izp�sob�me polohu - op�t v z�vislosti na �ase.
//
//void simulate(float dt)
//
//{
//
//vel += (force / m) * dt;// Zm�na rychlosti je p�i�tena k aktu�ln� rychlosti
//
//pos += vel * dt;// Zm�na polohy je p�i�tena k aktu�ln� poloze
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
//int numOfMasses;// Po�et objekt� v z�sobn�ku
//
//Mass** masses;// Objekty jsou uchov�v�ny v jednorozm�rn�m poli ukazatel� na objekty
//
//Simulation(int numOfMasses, float m)// Konstruktor vytvo�� objekty s danou hmotnost�
//
//{
//
//this->numOfMasses = numOfMasses;// Inicializace po�tu
//
//masses = new Mass*[numOfMasses];// Alokace dynamick� pam�ti pro pole ukazatel�
//
//for (int a = 0; a < numOfMasses; ++a)// Projdeme v�echny ukazatele na objekty
//
//masses[a] = new Mass(m);// Vytvo��me objekt a um�st�me ho na m�sto v poli
//
//}
//
//~Simulation()// Sma�e vytvo�en� objekty
//
//{
//
//release();
//
//}
//
//virtual void release()// Uvoln� dynamickou pam�
//
//{
//
//for (int a = 0; a < numOfMasses; ++a)// Sma�e v�echny vytvo�en� objekty
//
//{
//
//delete(masses[a]);// Uvoln� dynamickou pam� objekt�
//
//masses[a] = NULL;// Nastav� ukazatele na NULL
//
//}
//
//delete(masses);// Uvoln� dynamickou pam� ukazatel� na objekty
//
//masses = NULL;// Nastav� ukazatel na NULL
//
//}
//
//Mass* getMass(int index)// Z�sk�n� objektu s ur�it�m indexem
//
//{
//
//if (index < 0 || index >= numOfMasses)// Pokud index nen� v rozsahu pole
//
//return NULL;// Vr�t� NULL
//
//return masses[index];// Vr�t� objekt s dan�m indexem
//
//}
//
//Proces simulace se skl�d� ze t�� krok�:
//
//   1. Init() nastav� s�ly na nulu
//   2. Solve() znovu aplikuje s�ly
//   3. Simulate(float dt) posune objekty v z�vislosti na �ase
//
//virtual void init()// Tato metoda zavol� init() metodu ka�d�ho objektu
//
//{
//
//for (int a = 0; a < numOfMasses; ++a)// Proch�z� objekty
//
//masses[a]->init();// Zavol�n� init() dan�ho objektu
//
//}
//
//virtual void solve()
//
//{
//
//// Bez implementace, proto�e nechceme v z�kladn�m z�sobn�ku ��dn� s�ly
//
//// Ve vylep�en�ch z�sobn�c�ch, bude tato metoda nahrazena, aby na objekty p�sobila n�jak� s�la
//
//}
//
//virtual void simulate(float dt)// V�po�et v z�vislosti na �ase
//
//{
//
//for (int a = 0; a < numOfMasses; ++a)// Projdeme v�echny objekty
//
//masses[a]->simulate(dt);// V�po�et nov� polohy a rychlosti objektu
//
//}
//
//V�echny tyto metody jsou vol�ny v n�sleduj�c� funkci.
//
//virtual void operate(float dt)// Kompletn� simula�n� metoda
//
//{
//
//init();// Krok 1: vynulov�n� sil
//
//solve();// Krok 2: aplikace sil
//
//simulate(dt);// Krok 3: vypo��t�n� polohy a rychlosti objekt� v z�vislosti na �ase
//
//}
//
//};
//
//Nyn� m�me jednoduch� simula�n� engine. Je zalo�en� na matematick� knihovn�. Obsahuje t��dy Mass a Simulation. Pou��v� b�nou Eulerovu metodu na v�po�et simulace. Te� jsme p�ipraveni na v�voj aplikac�. Aplikace, kterou budeme vyv�jet vyu��v�:
//
//   1. Objekty s konstantn� hmotnost�
//   2. Objekty v gravita�n�m poli
//   3. Objekty spojen� pru�inou s n�jak�m bodem
//
//
//
//   Ovl�d�n� simulace aplikac�
//
//P�edt�m ne� nap�eme n�jakou simulaci, m�li bychom v�d�t, jak se t��dami zach�zet. V tomto tutori�lu jsou simula�n� a aplika�n� ��sti odd�leny do dvou samostatn�ch soubor�. V souboru s aplika�n� ��st� je funkce Update(), kter� se vol� opakovan� p�i ka�d�m nov�m framu.
//
//void Update (DWORD milliseconds)// Aktualizace pohybu
//
//{
//
//// O�et�en� vstupu z kl�vesnice
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
//DWORD milliseconds je �as, kter� uplynul od p�edchoz�ho vol�n� funkce. Budeme po��tat �as p�i simulac�ch na milisekundy. Pokud bude simulace sledovat tento �as, p�jde stejn� rychle jako v re�ln�m �ase. K proveden� simulace jednodu�e zavol�me funkci operate(float dt). P�edt�m ne� ji zavol�me mus�me zn�t hodnotu dt. Proto�e ve t��d� Simulation nepou��v�me milisekundy, ale sekundy, p�evedeme prom�nnou milliseconds na sekundy. Potom pou�ijeme prom�nnou slowMotionRatio, kter� ud�v�, jak m� b�t simulace zpomalen� vzhledem k re�ln�mu �asu. Touto prom�nnou d�l�me dt a dostaneme nov� dt. P�id�me dt k prom�nn� timeElapsed, kter� ud�v� kolik �asu simulace u� ub�hlo (neud�v� tedy re�ln� �as).
//
//float dt = milliseconds / 1000.0f;// P�epo��t� milisekundy na sekundy
//
//dt /= slowMotionRatio;// D�len� dt zpomalovac� prom�nnou
//
//timeElapsed += dt;// Zv�t�en� uplynul�ho �asu
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
//1. Objekt s konstantn� rychlost�
//
//Objekt s konstantn� rychlost� nepot�ebuje p�soben� extern� s�ly. Pouze vytvo��me objekt a nastav�me jeho rychlost na (1.0f, 0.0f, 0.0f), tak�e se bude pohybovat po ose x rychlost� 1 m/s. T��du ConstantVelocity odvod�me od t��dy Simulation.
//
//class ConstantVelocity : public Simulation
//
//{
//
//public:
//
//// Konstruktor nejd��ve pou�ije konstruktor nad�azen� t��dy, aby vytvo�il objekt o hmotnosti 1 kg
//
//ConstantVelocity() : Simulation(1, 1.0f)
//
//{
//
//masses[0]->pos = Vector3D(0.0f, 0.0f, 0.0f);// Nastav�me polohu objektu na po��tek
//
//masses[0]->vel = Vector3D(1.0f, 0.0f, 0.0f);// Nastav�me rychlost objektu na (1.0f, 0.0f, 0.0f) m/s
//
//}
//
//};
//
//
//
//
//
//Aplikace s�ly
//
//P�i simulac�ch s konstantn� rychlost� jsme nepou�ili s�lu p�sob�c� na objekt, proto�e v�me, �e pokud s�la p�sob� na objekt, tak m�n� jeho rychlost. Pokud chceme pohyb s prom�nlivou rychlost� pou�ijeme vn�j�� s�lu. Nejd��ve mus�me v�echny p�sob�c� s�ly se��st, abychom dostali v�slednou s�lu, kterou v simula�n� f�zi aplikujeme na objekt.
//
//Dejme tomu, �e chcete pou��t na objekt s�lu 1 N ve sm�ru x. Pak do solve() nap�ete:
//
//mass->applyForce(Vector3D(1.0f, 0.0f, 0.0f));
//
//Pokud chcete nav�c p�idat s�lu 2 N ve sm�ru y, nap�ete:
//
//mass->applyForce(Vector3D(1.0f, 0.0f, 0.0f));
//mass->applyForce(Vector3D(0.0f, 2.0f, 0.0f));
//
//Na objekt m��ete pou��t libovoln� mno�stv� sil, libovoln�ch sm�r�, abyste ovlivnili pohyb. V n�sleduj�c� ��sti pou�ijeme jednoduchou s�lu.
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
//MotionUnderGravitation vytvo�� objekt a nech� na n�j p�sobit s�lu. Touto silou bude pr�v� gravitace, kter� se vypo��t� vyn�soben�m hmotnosti objektu a gravita�n�ho zrychlen�:
//
//F = m * g
//
//Gravita�n� zrychlen� na Zemi odpov�d� 9.81 m/s^2. To znamen�, �e objekt p�i voln�m p�du zrychl� ka�dou sekundu o 9.81 m/s dokud na n�ho nep�sob� ��dn� jin� s�la ne� gravitace. M��e j� b�t odpor vzduchu, kter� p�sob� v�dycky, ale to sem nepat��.
//
//
//
//
//
//
//3. Objekt spojen� pru�inou s bodem
//
//V tomto p��klad� chceme spojit objekt se statick�m bodem. Pru�ina by m�la objekt p�itahovat k bodu upevn�n� a tak zp�sobovat oscilaci objektu. V konstruktoru nastav�me bod upevn�n� a pozici objektu.
//
//class MassConnectedWithSpring : public Simulation
//
//{
//
//public:
//
//float springConstant;// ��m vy��� bude tato konstanta, t�m tu��� bude pru�ina
//
//Vector3D connectionPos;// Bod ke kter�mu bude objekt p�ipojen
//
//// Konstruktor nejd��ve pou�ije konstruktor nad�azen� t��dy, aby vytvo�il 1 objekt o hmotnosti 1kg
//
//MassConnectedWithSpring(float springConstant) : Simulation(1, 1.0f)
//
//{
//
//this->springConstant = springConstant;// Nastaven� tuhosti pru�iny
//
//connectionPos = Vector3D(0.0f, -5.0f, 0.0f);// Nastaven� pozice upev�ovac�ho bodu
//
//masses[0]->pos = connectionPos + Vector3D(10.0f, 0.0f, 0.0f);// Nastaven� pozice objektu na 10 metr� napravo od bodu, ke kter�mu je uchycen
//
//masses[0]->vel = Vector3D(0.0f, 0.0f, 0.0f);// Nastaven� rychlosti objektu na nulu
//
//}
//
//Rychlost objektu je nula a jeho pozice je 10 metr� napravo od �chytu, tak�e se bude pohybovat ze za��tku sm�rem doleva. S�la pru�iny se d� zapsat jako
//
//F = -k * x
//virtual void solve()// U�it� s�ly pru�iny
//
//{
//
//// Pou�ijeme s�lu na v�echny objekty (zat�m m�me jenom jeden, ale to se m��e do budoucna zm�nit)
//
//for (int a = 0; a < numOfMasses; ++a)
//
//{
//
//Vector3D springVector = masses[a]->pos - connectionPos;// Nalezen� vektoru od pozice objektu k �chytu
//
//masses[a]->applyForce(-springVector * springConstant);// Pou�it� s�ly podle uveden�ho vzorce
//
//}
//
//}
//
//};
//k je tuhost pru�iny a x je vzd�lenost od �chytu. Z�porn� hodnota u k zna��, �e jde o p�ita�livou s�lu. Kdyby bylo k kladn�, tak by pru�ina objekt odpuzovala, co� zcela jist� neodpov�d� skute�n�mu chov�n�.
//
//
//
//
//*/