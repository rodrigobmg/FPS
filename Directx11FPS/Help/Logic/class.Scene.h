#ifndef H_Scene
#define H_Scene
#include <string>
#include <exception>

/*
	Najprv sa vola construc, kde sa vytvoria objekty, zakaldne veci dat ale aj dalsie.
	Potom sa vola precache kde maju byt len subory ako v amxmodx.
	Potom sa vola INIT kde sa daju dalsie veci na inicializaciu.
	Frame sa vola kazdy snimok z D.
	End sa vola na konci sceny, scena sa da znova obnovit.
	Descutor pri zniceni sceny.
*/

class Scene
{
	friend class SceneManager;

protected:
	virtual void Stop() { } // prikaz zastavy scenu
	virtual bool Precache() { return true; }
	virtual void Init() = 0;
	virtual void Frame(double d) = 0;
	virtual void End() = 0;
	// class Pause : public std::exception { };
	// class Remove : public std::exception { };
	// class Break : public std::exception { };

public :
	virtual const char* getName() { return "Scene"; }
};

#endif