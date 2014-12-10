#include "../../../Global.h"
#include "class.Window.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <exception>

using namespace std;

// MoveWindow(hWnd, 0, 0, iWidth, iHeight, TRUE);

Window::Window(LPCWSTR name, WNDPROC proc, Screen *info){
	Window();
	m_wc.lpszClassName	= name;
	m_wc.lpfnWndProc	= proc;
	m_screeninfo = info;
	Create();	
}
Window::Window() {
	// Nove vlastnosti okna
	m_wc.style			= CS_HREDRAW | CS_VREDRAW;// | CS_OWNDC;
	m_wc.cbClsExtra		= 0;
	m_wc.cbWndExtra		= 0;
	m_wc.hInstance		= GetModuleHandle(NULL);
	m_wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	m_wc.hIconSm		= m_wc.hIcon;
	m_wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
	m_wc.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	m_wc.lpszMenuName	= NULL;
	m_wc.lpszClassName	= L"App";
	m_wc.lpfnWndProc	= (WNDPROC) DefaultWndProc;
	m_wc.cbSize			= sizeof(WNDCLASSEX);
}
LRESULT Window::DefaultWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
void Window::Create()
{
	// Register the window class.
	RegisterClassEx(&m_wc);

	// Determine the resolution of the clients desktop screen.
	UINT posX, posY;
	GetWinPosition(&posX, &posY);

	// Create the window with the screen settings and get the handle to it.
	m_okno = CreateWindowEx(WS_EX_APPWINDOW, m_wc.lpszClassName, m_wc.lpszClassName, 
							WS_OVERLAPPEDWINDOW, //WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, m_screeninfo->width, m_screeninfo->height, 
							NULL, NULL, m_wc.hInstance, NULL);

	if(m_okno == NULL) {
		throw new exception("Nepodarilo sa vytvorit okno.");
	}
}
void Window::GetWinPosition(UINT *posX, UINT *posY) {
	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(m_screeninfo->fullscreen)
	{	
		GetDesktopSize();

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = m_screeninfo->width;
		dmScreenSettings.dmPelsHeight = m_screeninfo->height;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		*posX = 0;
		*posY = 0;
	} else {
		// Place the window in the middle of the screen.
		*posX = (GetSystemMetrics(SM_CXSCREEN) - m_screeninfo->width)  / 2;
		*posY = (GetSystemMetrics(SM_CYSCREEN) - m_screeninfo->height) / 2;
	}
}
void Window::GetDesktopSize() {	
	// Uloz tie hodnoty
	// TODO Toto zisti najvacsie rozlizenie a ulozi to, ale to musime overit ci grafika to splna
	// A az takto to poslat
	m_screeninfo->width = GetSystemMetrics(SM_CXSCREEN);
	m_screeninfo->height = GetSystemMetrics(SM_CYSCREEN);
}
void Window::Refresh() {
	InvalidateRect(m_okno, NULL, FALSE);
}
void Window::Cursor(bool x) {
	ShowCursor(x);
}
void Window::Show(ShowType i) {		
	ShowWindow(m_okno, SW_SHOW); // TODO i
	SetForegroundWindow(m_okno);
	SetFocus(m_okno);
	//if(i > 0) SetForegroundWindow(m_okno);
	//if(i > 0) SetFocus(m_okno);
}
void Window::Error(LPCWSTR txt) { // ak je 0 tak je to bezspecny koniec ...
	MessageBox(m_okno, txt, L"Error", MB_OK|MB_ICONERROR ); // gOkno zabezspeci nevypnutie
}
Screen** Window::getScreenInfo() {
	return &m_screeninfo;
}
HWND Window::getWindow() {
	return m_okno;
}
WNDCLASSEX& Window::getWND() {
	return m_wc;
}
LPCWSTR Window::getName() {
	return m_wc.lpszClassName;
}
void Window::SetName(LPCWSTR name) {
	m_wc.lpszClassName = name;
}
void Window::SetWNDProc(WNDPROC proc) {
	m_wc.lpfnWndProc = proc;
}
Window::~Window() {
	Cursor(true);
	if(m_screeninfo->fullscreen) {
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_okno);
	m_okno = NULL;

	// Remove the application instance.
	UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
	m_wc.hInstance = NULL;
}