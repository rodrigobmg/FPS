#pragma once
#include "../Base/DesignPattern.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class WinDebug 
{
	SINGLETON(WinDebug)

	void Header();
	void Pip();
	void Ciara();
	WinDebug();

public :
	virtual void Echo(LPCTSTR p);
	virtual void Error(LPCTSTR p);
	virtual void Warning(LPCTSTR p);
};