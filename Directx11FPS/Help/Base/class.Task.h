#pragma once
#include "class.Event.h"
#include "class.Module.h"
#include "class.Timer.h"
#include "class.HeapTree.h"

class TaskManager;
class Task;
// Ak chceme multytask, jednoducho do v Run sa zase registrujeme


class Task : public IRunnable
{  
private:
	double m_runtime;
	friend class TaskManager;
	friend class HeapTree<Task*>;

	bool Heapsort(Task *a) {
		return m_runtime < a->m_runtime;
	}

public:
	virtual void Run() = 0;
	virtual ~Task() {}

};


class TaskManager : public Module
{
private:
	HeapTree<Task*> m_tasks; // lepsie pouzit HEAP, zoznam udrziavame zoradeny
	Timer m_time;

	void RunTaskCheck() {
		double time = m_time.GetAbsoluteTime();
		Task *temp;

		// Spustame a kontrolujeme vsetke
		while(!m_tasks.IsEmpty()) {
			temp = m_tasks.GetTop();
			if(temp->m_runtime < time) {
				temp->Run();
				m_tasks.RemoveTop();
			} else {
				break; // zoznam je usporiadany
			}
		}
	}

protected:
	virtual void Frame() {
		RunTaskCheck();
	}

public:
	void Add(Task* a, double time) {
		a->m_runtime = time + m_time.GetAbsoluteTime(); 
		m_tasks.Add(a);
	}
	TaskManager* operator+ ( Task *a){
		Add(a, -0.1);
		return this;
	}
	~TaskManager() {
		m_tasks.RemoveAll();
	}
};