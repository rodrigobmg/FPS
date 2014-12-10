#pragma once

#include "Base/class.BaseDirect.h"
#include "Base/class.Window.h"
#include "Base/inter.IWNDProc.h"
#include "../Graphic/struct.GraphicOptions.h"
#include "../Graphic/Inter/inter.IGraphic.h"

class Direct : 
	public BaseDirect, 
	public IGraphicDeviceManager, 
	public IWNDProc
{
public:
	// Ovladaj direct
	Direct();
	virtual void Initialize(HWND win, GraphicOptions *a);
	void BeginScene();
	void EndScene();

	// Vykresluj

	// Dalsie veci
	inline GraphicOptions* getOption() { return m_graphicoption; }

protected:
	// Callbacky
	virtual void WND_SIZE(WPARAM& wparam);
	void ResetOptions(); // nieco sa zmenilo v GraphicOptions

	// Prepisane metody od basedirect
	virtual void SetDirectDesc();
	virtual HWND getWindow();
	virtual void DefaultMatrixes(); 
	virtual void FindBestModeForMonitor(IDXGIOutput* adapterOutput, UINT screenWidth, UINT screenHeight);

private:
	GraphicOptions* m_graphicoption;
	HWND m_win;
};