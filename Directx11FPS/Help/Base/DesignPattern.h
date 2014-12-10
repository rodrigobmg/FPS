#ifndef H_DesignPattern
#define H_DesignPattern

#define STATIC(name) name::getInstance()
#define pure(name) virtual name = 0; 
#define inter(name) virtual name { } 
#define implements public
#define setter( x, y )   inline void set##y( x t )   { m_##y = t; };
#define getter( x, y )   inline x get##y()           { return m_##y; };
#define setget( x, y )   setter( x, y ) getter( x, y )					// hodnotu ziskas aj nastavit
#define prosetget( x, y )  protected: x m_##y; public : setget( x, y ) // zadefinuje, set, get pre hodnotu
#define prisetget( x, y )  private: x m_##y; public : setget( x, y )	// zadefinuje, set, get pre hodnotu



#define SINGLETON(name) \
	public: \
			static name& getInstance() {\
				static name singleton; \
				return singleton; \
		  }; \
	private: \
		name(const name&);  \
		name& operator=(const name&); \
	

/*#define STATICINIT(name) \
	private: \
		name() {}		
		name(const name&);  \
		name& operator=(const name&); \
*/

#endif


/*++ Priklady 

abstract class Pouzivam : implements IWNDProc, implements IGraphic {

}
interface class Pouzivam : implements IWNDProc, implements IGraphic {
	pure( void FullscreenToggle() )
	inter( void Resize() )
public:
	setter(bool, Meno)
	getter(bool, Meno)
	setget(bool, MenoNaziskan)
}

*/