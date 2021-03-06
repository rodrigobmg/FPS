#ifndef H_input
#define H_input
#include <windows.h>
#include "../Base/DesignPattern.h"
#include "../App/class.Game.h"
#include "inter.IWNDProc.h"
#include <stack>
using namespace std;

// MK_MBUTTON, MK_SHIFT
// xPos = GET_X_LPARAM(lParam); 
// yPos = GET_Y_LPARAM(lParam); 

class Input : public Module, public IWNDProc
{
private : 
	// Klavesnica
	Timer m_timer;
	bool m_button[256];
	bool m_press[256];
	bool m_pressold[256];
	double m_time[256];
	
	// Myska
	POINT m_pos;
	POINT m_oldpos;
	bool m_prave;
	bool m_lave;

	void ProcessPress();
	void PressStart(int i);

protected:
	virtual void WND_KEYUP(WPARAM& wparam);
	virtual void WND_KEYDOWN(WPARAM& wparam);
	virtual void WND_LBUTTONDOWN(WPARAM& wparam);
	virtual void WND_LBUTTONUP(WPARAM& wparam);
	virtual void WND_RBUTTONDOWN(WPARAM& wparam);
	virtual void WND_RBUTTONUP(WPARAM& wparam);

public :
	// Klavesnica
	bool getKeydown(WPARAM key);			// ci je stistknute, prave stlaca
	bool getPress(WPARAM key);				// prave stlaca R ale event sa spusti len raz az kym nepusti
	bool getPress(WPARAM key, double time); // vrati true ak za 1 sec bolo raz stlacene, potom to blokuje ?
	double getTimePress(WPARAM key);

	// Myska
	bool getPrave();
	bool getLave();
	bool getStredne();
	
	POINT getPos();
	POINT getRozdiel();
	//int getClicksPerSeconds(float sec); // ci klika za 1 sec, 100x

	Input();
	void Reset();
	virtual void Frame();
};

#endif