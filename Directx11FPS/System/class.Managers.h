#ifndef H_SceneManager
#define H_SceneManager
#include "../Help/App/class.Game.h"
#include "../Help/Math/class.vec.h"
#include "../Scenes/class.Scene.h"
#include "../Help/Logic/OCtree/class.Octree.h"
#include "../Help/App/class.Direct.h"
extern class Entity;

class OCtreeManager : public Module, public OCTree 
{
	void ResetEntities();
	//OCTree* m_active; // TODO visible octree
	//OCTree* m_active; // TODO octre osiby

public:
	OCtreeManager() {
		Set(NULL);
	}
	OCtreeManager(vec &absmin, vec &absmax) { 
		Set(NULL);
		setSizeMM(absmin, absmax);
	}
	virtual void Frame();
};


class PhysicsManager : public Module {
public:
	PhysicsManager();
	virtual void Frame();
	vec gravition;

private :
	double DeltaTime;

	void Gravitation();	
	void TestCollisions();
	void BspCollision(Entity *a);
	void ColAnswer(Entity *a, Entity *b);
	void FinalForce();	
};



class RenderManager : public Module
{
	private:
		bool m_preframe;

	public:
		RenderManager(bool pre) { m_preframe = pre; }
		virtual void Frame() {
			if(m_preframe) { 
				gDirect->BeginScene();
			} else { 
				gDirect->EndScene();
			}
		}
};

class SceneManager : public Module, protected DynArray<Scene*> 
{	
protected:
	virtual void Frame();
	void Init();
	void Precache();
	void End();

public :
	SceneManager();
	void Clear();
	~SceneManager();
};




#endif