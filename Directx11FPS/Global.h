#ifndef H_Global
#define H_Global

#define DEBUG	true

#define SWAP( a, b ) { UINT _temp = a; a = b; b = _temp; }

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#pragma warning( disable : 4091 )

#if defined(DEBUG) || defined(_DEBUG)
	#include <exception>
	#ifndef V
	#define V(x)           { hr = (x); if( FAILED(hr) ) { throw std::exception(); } }
	#endif
	#ifndef V_RETURN
	#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { throw new std::exception(); } }
	#endif
#else
	#ifndef V
	#define V(x)           { hr = (x); }
	#endif
	#ifndef V_RETURN
	#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
	#endif
#endif

class Input;
class Game;
class Direct;
class CBaseCamera;

// Globalne
extern Input* gInput;
extern Game* gGame;
extern Direct* gDirect;
extern CBaseCamera* gCamera;

#endif