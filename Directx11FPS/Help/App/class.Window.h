#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "struct.Screen.h"

class Window
{
private:
	static LRESULT DefaultWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND m_okno;
	WNDCLASSEX m_wc;
	Screen *m_screeninfo;

protected:
	virtual void Create();
	virtual void GetWinPosition(UINT *posX, UINT *posY);
	virtual void GetDesktopSize();

	void Refresh();
	void Error(LPCWSTR txt);
	void Cursor(bool x);

	WNDCLASSEX& getWND();
	Screen** getScreenInfo();

	enum showWindowType {
		hide            =    SW_HIDE,
		normal          =    SW_SHOWNORMAL,
		showNormal      =    normal,
		showMinimized   =    SW_SHOWMINIMIZED,
		showMaximized   =    SW_SHOWMAXIMIZED,
		maximize        =    showMaximized,
		showNoActivate  =    SW_SHOWNOACTIVATE,
		show            =    SW_SHOW,
		minimize        =    SW_MINIMIZE,
		showMinNoActive =    SW_SHOWMINNOACTIVE,
		showNA          =    SW_SHOWNA,
		restore         =    SW_RESTORE,
		showDefault     =    SW_SHOWDEFAULT,
		forceMinimize   =    SW_FORCEMINIMIZE
	};
	void Show(showWindowType i = showNormal);

public: 
	Window(LPCWSTR name, WNDPROC proc, Screen *info);
	Window();
	~Window();
	
	HWND getWindow();
	LPCWSTR getName();
};