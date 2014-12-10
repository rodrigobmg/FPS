#pragma once
#include "class.Event.h"
#include "../Structures/Trees/class.HeapTree.h"

class Task;
extern class TaskManager;

// Ak chceme multytask, jednoducho do v Run sa zase registrujeme

class Task : public IRunnable
{  
private:
	double m_runtime;
	friend class TaskManager;

protected:
	virtual void Destroy() {}

public:
	virtual void Run() = 0;
	~Task() { Destroy(); }

	bool Heapsort(Task *a) {
		return m_runtime < a->m_runtime;
	}
};
