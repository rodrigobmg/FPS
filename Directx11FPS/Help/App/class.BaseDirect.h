////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef H_BASEDIRECT
#define H_BASEDIRECT


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")


//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: Direct
////////////////////////////////////////////////////////////////////////////////

class BaseDirect
{
public:
	BaseDirect();
	~BaseDirect();
	
	void BeginScene(float color[4]);

	void TurnZBuffer(bool x);
	void TurnAlphaBlending(bool turn);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	D3DXMATRIX* GetProjectionMatrix();
	D3DXMATRIX* GetWorldMatrix();
	D3DXMATRIX* GetOrthoMatrix();
	const char* GetVideoCardName();
	int GetMemorySize();
	
protected:
	virtual void Initialize();
	virtual void SetDirectDesc();
	virtual void CreateDevice();
	virtual void CreateNext();
	virtual void GetGraphicCardInfo();
	virtual void FindBestModeForMonitor(IDXGIOutput* adapterOutput, UINT screenWidth, UINT screenHeight);
	virtual void DefaultMatrixes();
	virtual HWND getWindow() = 0;

	virtual void CreateRasterizerState();
	virtual void CreateDepthStencilState(bool zbuffer); // alias Z-bufferu
	virtual void CreateAlphaBlendingState(bool turn);

	// Vlastnosi s ktorymi sa inicializuje direct
	UINT m_numerator;
	UINT m_denominator;
	int m_videoCardMemory;
	char m_videoCardName[128];

	// Cleny pre Direct
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState[2];
	ID3D11BlendState* m_alphaBlendingState[2];
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	// Matice pre projekciu
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;

	// Vlastnosti Direct
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC m_depthStencilViewDesc;
	D3D11_TEXTURE2D_DESC m_depthBufferDesc;
	D3D11_VIEWPORT m_viewport;
};

#endif