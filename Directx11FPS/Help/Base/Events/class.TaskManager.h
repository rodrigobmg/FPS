#pragma once
#include "../class.Module.h"
#include "../System/class.Time.h"
#include "../Structures/Trees/class.HeapTree.h"

class TaskManager 
	: public Module
{
private:
	HeapTree<Task*> m_tasks; // lepsie pouzit HEAP, zoznam udrziavame zoradeny

	void RunTaskCheck() {
		double time = Time::getInstance().GetAbsolute();
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
	virtual void Frame(SFrame& s) {
		RunTaskCheck();
	}

public:
	TaskManager() {

	}
	inline void Add(Task* a, double time) {
		a->m_runtime = time + Time::getInstance().GetAbsolute(); 
		m_tasks.Add(a);
	}
	inline TaskManager* operator+ ( Task *a){
		Add(a, -0.1);
		return this;
	}
	~TaskManager() {
		m_tasks.RemoveAll();
	}
};