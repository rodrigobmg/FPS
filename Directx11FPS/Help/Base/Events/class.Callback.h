#pragma once
#include "class.Event.h"

/*
	Event lol = new EventCallbackClass<g, Open()>();
	lol->setArgs(6)->Run();
	
	lol->Compare(g, Open()); / /true

	Event lol = new EventCallbackClass<g, Open(), 6>();
	lol->Run();

	Event lol = new EventCallbackFunction<Open(), 6>();
	lol->Run();

	class CarEvent : public Event {  
	public:  
		virtual void Run() { 
			// Co sa ma spravit
		}
	};
	Event lol = new CarEvent();
	lol->setArgs(6)->Run();


	class EventTime : public Event {  
		float lasttime= 0;
	
	public:  
		virtual void Run() { 
			// Co sa ma spravit
			lasttime++;
			lasttime++;
		}
	};

*/

class EventCallbackFunction : public Event  
{  
public:  
    EventCallbackFunction(void(*func)(Event *)) {  
        m_Func = func;  
    }
	EventCallbackFunction(void(*func)(Event *), EventArgs *args) {  
        m_Func = func;  
		setArgs(args);
    } 
	virtual void Run() { 
		(m_Func)((Event*)this);  
	}
	bool Compare(void(*func)(ARG*)) {
		return (*func == *m_Func );
	}

private:   
    void (*m_Func)(Event*);  
};  

template<class T> class EventCallbackClass : public Event  
{  
public:  
    EventEasyClass(T* obj, void(T::* func)(Event *)) {  
        m_Obj  = obj;  
        m_Func = func;  
    }
	EventEasyClass(T* obj, void(T::* func)(Event *), EventArgs *args) {  
        m_Obj  = obj;  
        m_Func = func;  
		setArgs(args);
    } 
	virtual void Run() { 
		(m_Obj->*m_Func)((Event*)this);  
	}
	bool Compare(T* obj, void(T::* func)(ARG*)) {
		return ( *obg == *m_Obj && *func == *m_Func );
	}

private:  
    T* m_Obj;  
    void (T::* m_Func)(Event*);  
};  



// http://www.gotw.ca/gotw/083.htm
class CallbackBase {
public:
	virtual void operator()() const { };
	virtual ~CallbackBase() {}
};


template< class T >
class Callback : public CallbackBase {
public:
	typedef void (T::*Func)();
	Callback( T& t, Func func ) : object(&t), f(func) { }
	void operator()() const { (object->*f)(); }

private:
	T* object;
	Func f;
};


template<typename T >
Callback<T> make_callback( T& t, void (T::*f) () ) {
	return Callback<T>( t, f );
}


// list< Callback< Widget > > l;
// l.push_back( make_callback( w, &Widget::SomeFunc ) );