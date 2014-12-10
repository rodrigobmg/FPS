#ifndef H_EntPhysics
#define H_EntPhysics
#include "../../Help/Math/class.vec.h"
#include "../../Help/Shapes/Shapes.h"

class Entity;
extern class ColAnswer;

class EntPhysicsForce {
protected:	
	vec m_force;

public: 
	virtual void AddForce(vec f) { m_force += f; }
	virtual void ForceInit() { m_force = 0.0f; }
};

class EntPhysics : public EntPhysicsForce
{
protected:
	bool m_solid;
	Shape::Base* m_shape;			
	vec m_velocity;
	vec m_angle;
	vec m_accelaration;
	float m_mass;
	float m_gravity;
	bool m_moved;

public: 
	// TODO Ak chceme think, Touch a dalsie CallBacky schovat
	// Treba do manazera pridat FRIEND a potom mozme volat protected cast
	virtual void Think(double d) { }
	virtual void Touch(Entity *ent, ColAnswer *col) { }

	virtual float getGravity() { return m_gravity; }
	virtual bool getMoved() { return m_moved; }
	virtual bool isSolid() { return m_solid; }
	virtual vec& getAngle();

	virtual Shape::Base* getShape() { return m_shape; }
	virtual void setShape(Shape::Base *s) { m_shape = s; }
	virtual float getKineticEnergy() { return 0.5f * m_mass * m_velocity.Length2(); } // ziskaj aktualnu kineticku energiu

	EntPhysics(Shape::Base *s) { 
		setShape(s); 
		m_solid = false;
		m_moved = false;
		m_mass = 1.0f;
		m_gravity = 1.0f;
	};
	EntPhysics() {
		EntPhysics::EntPhysics( new Sphere(vec(), 1.0f) );
	}
	~EntPhysics() {
		SAFE_DELETE(m_shape);
	}
};

/*
	public void InitThink() {
		fv.set(0.0, 0.0, 0.0);
	}
	public void ApplyGravity() {
		AddForce(vec(0.0, -9.8, 0.0) );
	}
	public void AddForce(vec f) { // Normalne aplikovat kazdu silu
		fv += f;
	}
	public void ForceFinalize() { // Na konci vsetke sily vynasob hmotnostou
		fv *= m;
	}
		public void CollisionFinalize() {
		// Prejdi kazdu jednu koliziu
		// Vypocitaj silu a aplikuj ju

		// Zaroven to sluzi ak ozoznam s objektamy v sktorymi je v interakcii, vsetko ide cez referenciu
	}
		public vec GetForce() { // vypocita akou silou zaposoby na objekt ak ide po uhlom A
		// A vypocitat, uhol medzi silou F a poziiciou objektu ak je na stole,..
		return f * sin(a);
	}
	// F = k*m  // struna
	
	// ak objekt ako gula sa pohybuje po stole velocity sa aplikuje aj na transakcny pohyb
	// preto by sme mohli definovat pre objekt TVAR a MATERLIA
	// s tvaru vieme aplikovat typ testu pre koliziu, dalej typ pohybum typ rovnic
	// s materialu zase konstanty, trenie, absorpcia, rezistencia
	
	
	// Kazda gulka ma velkost penetracie
	// Kazda stena material ma velkost pohlcovania
	// kazdy material ma velkost absorpcie pri naraze, inak spadne, a sila sa presunie na rotacny pohyb
	
	
	// koleso nieje kruh ale torgue, cize je tam deformacia podla toho je aj prilienavost
	// a podla toho sa upravuje akceleracia

	// ak su objekty prepojene aku ruka s telom
	// velocity z hlavneho objektu sa prenasa aj na ruku a ostatne casi

	// vo fyzike sa casto krat pocita kvadraticka, len kladna cast ma zmysel, spravit funkciu
	// clamp potrebjeme
	// remap potrebujeme
*/

#endif
