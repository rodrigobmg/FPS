#pragma once
#include "DirectFPS.h"
/*
	A 60%
	B 40%

*/

class TestVirtualFunctionsA : public Module {
public:
	class Test {
	public:
		virtual void function() {}
	};
	Test *a;

	TestVirtualFunctionsA() {
		a = new Test;
	}
	virtual void Frame() {
		long i;
		for(i=0; i < 100000000; i++) {
			a->function();
		}
	}
	~TestVirtualFunctionsA() {
		delete a;
	}
};

class TestVirtualFunctionsB : public Module {
public:
	class Test {
	public:
		void function() {}
	};
	Test *a;

	TestVirtualFunctionsB() {
		a = new Test;
	}
	virtual void Frame() {
		long i;
		for(i=0; i < 100000000; i++) { // 2 000 000 000 trva 1 sec
			a->function();
		}
	}
	~TestVirtualFunctionsB() {
		delete a;
	}
};