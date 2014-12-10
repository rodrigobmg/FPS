#pragma once
#include <iostream>

namespace Hud
{

// Zaklad
extern class Driver;

class Item
{
private:	
	friend class Driver;
	Driver* m_huddriver;

protected:
	virtual void Render() {}
	Driver* getDriver() { 
		return m_huddriver; 
	}

public:
	Item() {
		m_huddriver = 0;
	}
	virtual ~Item() { }
	bool operator < (const Item &) {
		return 0;
	}
	friend std::ostream& operator<<(std::ostream& out, Item& v){
		return out << "Item";
	}
};

}

/* 
 * HP hud 
	Vytvorit frame, tam sa budem pytat na hp cloveka a potom pouzijem obrazok ktory chcem.
	- Obrazky naloadovat a len ich meneit, netreba zase alokovat
	- Hud moze byt aktivny a podla premennej si ho spustime vo frame
*/