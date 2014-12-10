#pragma once
#include "../..//Base/DesignPattern.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class WinDebug 
{
	SINGLETON(WinDebug)

	void Header();
	void Pip();
	void Ciara();

public :
	virtual void Echo(const char* p);
	virtual void Error(const char* p);
	virtual void Warning(const char* p);
	virtual void MVSMsg(LPCWSTR p); // Send message to microsoft visual studio

private:
	WinDebug();
};