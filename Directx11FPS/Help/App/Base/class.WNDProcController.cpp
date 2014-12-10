#include "class.WNDProcController.h"

//#include "../DXUT/dxutcamera.h"
//extern CModelViewerCamera g_Camera;
IWNDProc* WNDProcController::m_instance = NULL;


LRESULT WNDProcController::GlobalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

	switch(uMsg) {
		case WM_ACTIVATE: { m_instance->WND_ACTIVATE(wParam); break; }
		case WM_CLOSE: { m_instance->WND_CLOSE(wParam); break; }
		case WM_CREATE: { m_instance->WND_CREATE(wParam); break; }
		case WM_DESTROY: { m_instance->WND_DESTROY(wParam); break; }
		case WM_MOUSEMOVE: { m_instance->WND_MOUSEMOVE(wParam); break; }
		case WM_KEYUP: { m_instance->WND_KEYUP(wParam); break; }
		case WM_KEYDOWN: { m_instance->WND_KEYDOWN(wParam); break; }
		case WM_TIMER: { m_instance->WND_TIMER(wParam); break; }
		case WM_USER: { m_instance->WND_USER(wParam); break; }
		case WM_PAINT: { m_instance->WND_PAINT(wParam); break; }
		case WM_QUIT: { m_instance->WND_QUIT(wParam); break; }
		case WM_SIZE: { m_instance->WND_SIZE(wParam); break; }
		case WM_LBUTTONDOWN: { m_instance->WND_LBUTTONDOWN(wParam); break; }
		case WM_LBUTTONUP: { m_instance->WND_LBUTTONUP(wParam); break; }
		case WM_RBUTTONDOWN: { m_instance->WND_RBUTTONDOWN(wParam); break; }
		case WM_RBUTTONUP: { m_instance->WND_RBUTTONUP(wParam); break; }
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}






void WNDProcMultyController::WND_ACTIVATE(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_ACTIVATE(wparam);
	}
} 
void WNDProcMultyController::WND_CLOSE(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_CLOSE(wparam);
	}
}
void WNDProcMultyController::WND_CREATE(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_CREATE(wparam);
	}
} 
void WNDProcMultyController::WND_DESTROY(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_DESTROY(wparam);
	}
}
void WNDProcMultyController::WND_MOUSEMOVE(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_MOUSEMOVE(wparam);
	}
} 
void WNDProcMultyController::WND_KEYUP(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_KEYUP(wparam);
	}
}
void WNDProcMultyController::WND_KEYDOWN(WPARAM& wparam) {
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_KEYDOWN(wparam);
	}
}
void WNDProcMultyController::WND_TIMER(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_TIMER(wparam);
	}
}
void WNDProcMultyController::WND_USER(WPARAM& wparam) {
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_USER(wparam);
	}
} 
void WNDProcMultyController::WND_PAINT(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_PAINT(wparam);
	}
} 
void WNDProcMultyController::WND_QUIT(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_QUIT(wparam);
	}
} 
void WNDProcMultyController::WND_SIZE(WPARAM& wparam) { 
	for(UINT i=0; i < m_array.GetSize(); i++) {
		m_array.GetAt(i)->WND_SIZE(wparam);
	}
}
void WNDProcMultyController::Add(IWNDProc* value) {
	m_array.Add(value);
}
WNDProcMultyController::~WNDProcMultyController() {
	for(UINT i=0; i < m_array.GetSize(); i++) {
		delete m_array.GetAt(i);
	}
}