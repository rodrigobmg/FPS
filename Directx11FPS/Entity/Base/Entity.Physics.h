#pragma once
#include "../../Help/Math/class.vec.h"
#include "../../Help/Shapes/Shapes.h"
#include "../../Help/Base/Structures/class.DynArray.h"

namespace Entity 
{ 
	extern class Base;

	//+ Interface pre spracovanie sily
	class IForce 
	{
		protected:	
			vec m_force;

		public: 
			virtual void Add(vec f) { 
				m_force += f; 
			}
			virtual void Init() { 
				m_force = 0.0f; 
			}
			virtual void Finalize() = 0;
	};
	
	//+ Interface pre koliziu
	class IPhysics;
	class ICollisionList : protected DynArray<IPhysics*> {
		protected: 	
			virtual void Init() { 
				this->RemoveAll();
			}
			virtual void Finalize() {
				// Prejdi kazdu jednu koliziu
				for(UINT i=0; i < GetSize(); i++) {
					// Vypocitaj silu a aplikuj ju
				}
			}
	};

	//+ Interface pre fyziku entity
	class IPhysics : protected IForce, protected ICollisionList
	{
	protected:
		bool m_solid;
		Shapes::Base* m_shape;			
		vec m_velocity;
		vec m_angle;
		vec m_accelaration;
		float m_mass;
		float m_gravity;
		bool m_moved;

		// Callbacky
		virtual void PreThink() { }
		virtual void Think(double d) { }
		virtual void PostThink() { }
		virtual void Touch(Entity::Base *ent) { }

		// Dalsie doplnky
		virtual void Init() { 
			IForce::Init();
			ICollisionList::Init();
		}
		virtual void Finalize() { 
			// Prejdi kolizie a aplikuj sily
			ICollisionList::Finalize();

			// Na konci vsetke sily vynasob hmotnostou
			m_force *= m_mass;
		}

	public: 
		
		virtual Shapes::Base* getShape() { return m_shape; }
		virtual void setShape(Shapes::Base *s) { m_shape = s; }
		virtual float getGravity() { return m_gravity; }
		virtual bool getMoved() { return m_moved; }
		virtual bool isSolid() { return m_solid; }
		virtual vec& getAngle();
		virtual float getKineticEnergy() { return 0.5f * m_mass * m_velocity.Length2(); } // ziskaj aktualnu kineticku energiu

		IPhysics(Shapes::Base *s) { 
			setShape(s); 
			m_solid = false;
			m_moved = false;
			m_mass = 1.0f;
			m_gravity = 1.0f;
		};

		// Ak je to zakaldna entita
		IPhysics() {
			//IPhysics(new Shapes::Sphere(vec(), 1.0f) );
		}

		~IPhysics() {
			SAFE_DELETE(m_shape);
		}
	};

}

/*
	// preto by sme mohli definovat pre objekt TVAR a MATERLIA
	// s tvaru vieme aplikovat typ testu pre koliziu, dalej typ pohybum typ rovnic
	// s materialu zase konstanty, trenie, absorpcia, rezistencia
	
	// Netuhe latky
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

