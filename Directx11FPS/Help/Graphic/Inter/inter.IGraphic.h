#pragma once
#include "../../Base/Structures/class.DynArray.h"
class Direct;
class IGraphicManager;

class IGraphic {
public:
	// Render something
	virtual void Render() = 0;

    // Performs application-defined tasks associated with freeing, releasing, or
    // resetting unmanaged resources.
	// alias ShutDown
    virtual void Dispose() {}
};

class IGraphicDevice {
protected:
	friend class IGraphicDeviceManager;
	IGraphicDevice() {} // abstract
	~IGraphicDevice(); // to unregister
	Direct *m_direct;

public:
	/// Called when DirectX device OnLostDevice happens.
	/// Usually unused, will sometimes call Dispose(), recreate in
	/// OnResetDevice then.
	virtual void OnDestroyDevice() {} // alias LostDevice
   
	/// Called when DirectX device OnResetDevice happens.
	/// Usually not used or only to regenerate disposed stuff fromOnLostDevice()
	virtual void OnCreateDevice() {} // alias onReset, po resete sa vytvori novy
};	

class IGraphicDeviceManager {
private:
	friend class IGraphicDevice; 
	Direct* m_direct;
	DynArray<IGraphicDevice*> m_array;

	void Add(IGraphicDevice* value);
	void Remove(IGraphicDevice* value);
	void RemoveIGraphic(IGraphicDevice* value);

protected:
	void OnDestroyDevice(); // ak sa zariadenie strati, kazdemu clenu sa prestavy pointer na Direct a OnLostDevice je len misc, inak ostatne veci budu dalej pokracovat
	void OnCreateDevice(Direct *direct);

public:
	IGraphicDeviceManager(Direct *direct);
	void AddIGraphic(IGraphicDevice* value);
};

/*
//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10SwapChainResized( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr = S_OK;

    V_RETURN( g_DialogResourceManager.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
    V_RETURN( g_SettingsDlg.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( g_fFOV, fAspectRatio, 0.1f, 1000.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 350 );
    g_SampleUI.SetSize( 170, 300 );

    return hr;
}

*/