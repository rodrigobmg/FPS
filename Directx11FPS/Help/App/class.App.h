#ifndef H_App
#define H_App
#include "Base/class.Window.h"
#include <vector>
#include "Base/inter.IWNDProc.h"

class App : public Window, public IWNDProc
{
private:
	void MainCycle();
	bool m_runnig;
	bool m_active;

protected :
	virtual bool CanRun(); 
	virtual bool Run() = 0;
	virtual void onEnd();
	virtual bool isActive();
	virtual void CMDParser(std::vector<std::string> parameters) = 0;

	// Zachytavame spravy s wnd proc cize callbacky
	virtual void WND_ACTIVATE(WPARAM& wparam);
	virtual void WND_CLOSE(WPARAM& wparam);
	virtual void WND_PAINT(WPARAM& wparam);
	virtual void WND_DESTROY(WPARAM& wparam);
	virtual void WND_QUIT(WPARAM& wparam);

public: 
	// Ovladaj celu aplikaciu
	virtual void Start();
	virtual void Stop();
	virtual void ForceToEnd(LPCTSTR txt = L"");
	void CheckStartParameters();
	App();
};


#endif
