#pragma once
#include "class.DynArray.h"
#include "class.Timer.h"

class Event;

/*
	Event ohlasovatelia ked nieco nastane
	Event manager spracuje ohlas
	Event sa spusti vtedy

	Kazdy ohlas ma iny typ a len rovnake typy si ho registruju.
*/

class IRunnable  {  
public:
	virtual void Run() = 0;  
}; 

class EventArgs {  
public: 
	void* m_Sender;  
};  
    
class Event  
{  
protected:	
	EventArgs* m_args;

public:  
	Event() {  m_args = NULL; }
	virtual Event* setArgs(EventArgs * arg) {
		m_args = arg;
		return this;
	}
	virtual EventArgs* getArgs() {
		return m_args;
	}
	virtual void Run() = 0;  
	virtual ~Event() {}
};  

class Events : public Event, public DynArray<Event*> 
{  
public:
	virtual void Run() {
		for(UINT i=0; i < GetSize(); i++) {
			GetAt(i)->Run();
		}
	}
	Events* operator+ ( Event* a){
		DynArray::Add(a);
		return this;
	}
};


/*
class DoorOpenEventArgs : public EventArgs {  
   public:  
      DoorOpenEventArgs(void* sender, lock_t lockType);  
  
   public:  
      lock_t m_LockType;  
};

class Door {  
   public:  
      void setOpenDoorEvent(Event* event);  
      void openWithKey();  
      void openWithDistanceControl();  
  
   private:  
      auto_ptr<Event> m_OpenDoorEvent;  
  
   public:  
      door_t m_DoorType;  
};  
void Door::setOpenDoorEvent(Event* event)  
{  
   m_OpenDoorEvent.reset(event);  
}
void Door::openWithKey()  
{  
   if (m_OpenDoorEvent.get()) {  
      DoorOpenEventArgs open_arg(this, KEY);  
      m_OpenDoorEvent->invoke(&open_arg);  
   }  
} 

*/
