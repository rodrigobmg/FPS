#include "class.WinDebug.h"
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <fstream>
using namespace std;

// Output a string to Visual Studio's output pane
// OutputDebugString(str.c_str());
WinDebug::WinDebug()
{
	// Tato krasna funkcia nam dovoluje v GUI vytvorit conzolu	
	AllocConsole() ;
	AttachConsole( GetCurrentProcessId() ) ;
	freopen( "CON", "w", stdout );
	Echo(L"Debug aktivovany:\n");
}

void WinDebug::Header(void) {
	static time_t rawtime;
	static struct tm * timeinfo;
	static char buffer[10];

	time( &rawtime );
	timeinfo = localtime(&rawtime);

	strftime(buffer, 10,"%H:%M:%S", timeinfo);
	printf("Cas %s:\n", buffer);
}
void WinDebug::Pip() {
	printf("\a");
}
void WinDebug::Ciara() {
	printf("------------------------------\n");
}
void WinDebug::Echo(LPCTSTR p)	{
	Header();
	printf("%s", p);
}
void WinDebug::Error(LPCTSTR p)	{
	printf("Chyba '%s' \n", p);
	MessageBox(NULL, p, L"ERROR", MB_OK|MB_ICONERROR );
}
void WinDebug::Warning(LPCTSTR p)	{
	Header();
	printf("!!! '%s' \n", p);
}