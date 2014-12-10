#ifndef H_Bullet
#define H_Bullet
//#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <exception>
#include "../Help/App/class.Game.h"
#include "../Help/App/class.Direct.h"
#include "../Global.h"

//#define VertexType D3DFVF_XYZ | D3DFVF_DIFFUSE

class cDebugDraw : public btIDebugDraw
{
	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) { }
	void reportErrorWarning(const char* warningString) { }
	void draw3dText(const btVector3& location,const char* textString) {  }
	void setDebugMode(int debugMode) {  }
	int	getDebugMode() const { return 0; };
	
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color) 
	{
		// Deklarujeme vertxi
		/*Vertex v[2]; 
		v[0].x = from.getX();
		v[0].y = from.getY();
		v[0].z = from.getZ();
		v[1].x = to.getX();
		v[1].y = to.getY();
		v[1].z = to.getZ();
		v[0].w = v[1].w = D3DCOLOR_ARGB(255, 255, 255, 255);
		
		gDirect->getDeviceContext()->SetFVF(VertexType);
		// Renderujeme
		gDirect->getDeviceContext()->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(Vertex));*/
	}
};

class BulletEngine : public Module
{
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	cDebugDraw *debugDraw;

	inline void Initialize() {
		try {
			broadphase = new btDbvtBroadphase();
			collisionConfiguration = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(collisionConfiguration);
			solver = new btSequentialImpulseConstraintSolver;
			dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
			debugDraw = new cDebugDraw;	
		} catch ( exception e ) {
			printf("Problem s bullet enginom.");
		}
	}

	btCollisionShape* groundShape;
	btCollisionShape* fallShape;
	btDefaultMotionState* groundMotionState;
	btRigidBody* groundRigidBody;
	btDefaultMotionState* fallMotionState;
	btRigidBody* fallRigidBody;

	void Start() {
		dynamicsWorld->setGravity(btVector3(0,-10,0));
 
		groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
        groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
        btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        groundRigidBody = new btRigidBody(groundRigidBodyCI);
        dynamicsWorld->addRigidBody(groundRigidBody);
 
        fallShape = new btSphereShape(1);
        fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,200,0)));
        btScalar mass = 1;
        btVector3 fallInertia(0,0,0);
        fallShape->calculateLocalInertia(mass,fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
        fallRigidBody = new btRigidBody(fallRigidBodyCI);
        dynamicsWorld->addRigidBody(fallRigidBody);
			
		debugDraw = new cDebugDraw;	
		dynamicsWorld->setDebugDrawer(debugDraw);
	}
public :
	BulletEngine() {
		Initialize();
		Start();
	}

	virtual void Frame() {
		btTransform trans;
		btVector3 vek;
		btVector3 a, b;
		dynamicsWorld->debugDrawWorld();
        dynamicsWorld->stepSimulation(1/60.f,10);
		fallRigidBody->getMotionState()->getWorldTransform(trans);
		fallRigidBody->getAabb(a, b);
		debugDraw->drawAabb(a, b, vek);
		debugDraw->drawSphere(90.0f, trans, vek);
	}

	~BulletEngine() {
        dynamicsWorld->removeRigidBody(groundRigidBody);
        delete groundRigidBody->getMotionState();
        delete groundRigidBody;
        delete groundShape;
		delete debugDraw;	
		 
		delete fallShape; 
		dynamicsWorld->removeRigidBody(fallRigidBody);
        delete fallRigidBody->getMotionState();
		delete fallRigidBody;
	}
};


#endif