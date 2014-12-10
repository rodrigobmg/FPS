#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
class WNDProcController;
class WNDProcMultyController;

// Interface pre spracovavanie sprav
class IWNDProc 
{
protected:
	// TODO virtual void WND_MSG(MSG msg, WPARAM& wparam, LPARAM& lparam); asi vyhodnejsie pre niektoru cast
	virtual void WND_ACTIVATE(WPARAM& wparam) { } // Sent when a window is activated or becomes the focus.
	virtual void WND_CLOSE(WPARAM& wparam) { } // Sent when a window is closed.
	virtual void WND_CREATE(WPARAM& wparam) { } // Sent when a window is first created.
	virtual void WND_DESTROY(WPARAM& wparam) { } // Sent when a window is about to be destroyed.
	virtual void WND_MOUSEMOVE(WPARAM& wparam) { } // Sent when the mouse has been moved.
	virtual void WND_KEYUP(WPARAM& wparam) { } // Sent when a key is released.
	virtual void WND_KEYDOWN(WPARAM& wparam) { } // Sent when a key is pressed.
	virtual void WND_TIMER(WPARAM& wparam) { }// Sent when a timer event occurs.
	virtual void WND_USER(WPARAM& wparam) { } //  Allows you to send messages.
	virtual void WND_PAINT(WPARAM& wparam) { } // Sent when a window needs repainting.
	virtual void WND_QUIT(WPARAM& wparam) { } // Sent when a Windows application is finally terminating.
	virtual void WND_SIZE(WPARAM& wparam) { } // Sent when a window has changed size.

	// Myska
	virtual void WND_LBUTTONDOWN(WPARAM& wparam) { }
	virtual void WND_LBUTTONUP(WPARAM& wparam) { }
	virtual void WND_RBUTTONDOWN(WPARAM& wparam) { }
	virtual void WND_RBUTTONUP(WPARAM& wparam) { }

	// Controlleri su priatelia
	friend class WNDProcController;
	friend class WNDProcMultyController;
};
