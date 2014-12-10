#pragma once
#include "inter.IWNDProc.h"
#include "../Base/class.DynArray.h"
#include <exception>

class WNDProcController : public IWNDProc {
private:
	static IWNDProc* m_instance;

public:
	static LRESULT WNDProcController::GlobalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	WNDPROC getWNDProc() { 
		m_instance = this;
		return (WNDPROC) (GlobalWndProc); 
	}
};


class WNDProcMultyController : public WNDProcController {
private:
	DynArray<IWNDProc*> m_array;

protected:
	void WND_ACTIVATE(WPARAM& wparam);
	void WND_CLOSE(WPARAM& wparam);
	void WND_CREATE(WPARAM& wparam);
	void WND_DESTROY(WPARAM& wparam);
	void WND_MOUSEMOVE(WPARAM& wparam); 
	void WND_KEYUP(WPARAM& wparam);
	void WND_KEYDOWN(WPARAM& wparam);
	void WND_TIMER(WPARAM& wparam);
	void WND_USER(WPARAM& wparam);
	void WND_PAINT(WPARAM& wparam);
	void WND_QUIT(WPARAM& wparam);
	void WND_SIZE(WPARAM& wparam);

public:
	void Add(IWNDProc* value);
	~WNDProcMultyController();

	// TODO class Break : public std::exception { };
};