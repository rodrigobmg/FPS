#include "class.App.h"
#include <string>
#include "../Base/String.h"
using namespace std;


App::App() {
	m_runnig = false;
	m_active = true;
}
void App::WND_ACTIVATE(WPARAM& wparam) { 
	m_active = !m_active; 
}
void App::WND_CLOSE(WPARAM& wparam) { 
	Stop();
}
void App::WND_PAINT(WPARAM& wparam) {
	ValidateRect(getWindow(), NULL);
}
void App::WND_DESTROY(WPARAM& wparam) {
	PostQuitMessage(0); //okno sa zatvori posli msg quit
}
void App::WND_QUIT(WPARAM& wparam) { 
	Stop();
}
void App::MainCycle() {
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while(CanRun()) {
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) { // prekladame spravy
			TranslateMessage( &msg );  // prekladame spravy
			DispatchMessage( &msg ); // uvolnujeme
		} else {
			if(Run()) {
				Refresh();	// nemozme priamo volat UpdateWindow ale toto mozme ..je take iste
			}
			Sleep(0); // pomahame windowsu sa vyrovnat s hrou
		}
	}
}
bool App::isActive() {
	return m_active;
}
bool App::CanRun() {
	return m_runnig;
}
void App::Start() {
	m_active = true;
	m_runnig = true;
	Show();
	MainCycle();
	onEnd();
}
void App::Stop() {
	m_runnig = false;
}
void App::ForceToEnd(LPCTSTR txt) {
	if(txt != L"") Error(txt);
	m_runnig = false;
	exit(0);
}
void App::CheckStartParameters() {	
	// Podla parametra v hre nastavuje program ...
	std::string parameter = ToString(GetCommandLine());

	// A delime 
	vector<std::string> R;
	StringExplode(parameter, "-", &R);
	vector<std::string>::iterator it;
	vector<std::string> a;

	for (it=R.begin() ; it < R.end(); it++ ) {
		StringExplode(*it, " ", &a);
		CMDParser(a);
	}
}
void App::onEnd() { }
void CMDParser(vector<std::string> parameters) { }