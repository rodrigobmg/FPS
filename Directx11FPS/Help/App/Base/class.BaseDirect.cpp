#include "class.BaseDirect.h"
#include "../../../Global.h"

BaseDirect::BaseDirect()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState[0] = m_depthStencilState[1] = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;

	// Initialize 
	ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
	ZeroMemory(&m_depthStencilViewDesc, sizeof(m_depthStencilViewDesc));
	ZeroMemory(&m_depthBufferDesc, sizeof(m_depthBufferDesc));

	m_numerator = 0;
	m_denominator = 1;
}

BaseDirect::~BaseDirect()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(m_swapChain) {
		m_swapChain->SetFullscreenState(false, NULL);
	}

	// Vymaz vsetko ostatne
	SAFE_RELEASE(m_rasterState)
	SAFE_RELEASE(m_depthStencilView)
	SAFE_RELEASE(m_depthStencilState[0])
	SAFE_RELEASE(m_depthStencilState[1])
	SAFE_RELEASE(m_alphaBlendingState[0])
	SAFE_RELEASE(m_alphaBlendingState[1])
	SAFE_RELEASE(m_depthStencilBuffer)
	SAFE_RELEASE(m_renderTargetView)
	SAFE_RELEASE(m_deviceContext)
	SAFE_RELEASE(m_device)
	SAFE_RELEASE(m_swapChain)
}

void BaseDirect::SetDirectDesc() 
{
	// Set to a single back buffer.
	m_swapChainDesc.BufferCount = 1;
	// Set the width and height of the back buffer.
	m_swapChainDesc.BufferDesc.Width = 800;
	m_swapChainDesc.BufferDesc.Height = 600;
	// Set regular 32-bit surface for the back buffer.
	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate of the back buffer.
	m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// Set the usage of the back buffer.
	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to.
	m_swapChainDesc.OutputWindow = getWindow();
	// Turn multisampling off.
	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;
	// Set to full screen or windowed mode.
	m_swapChainDesc.Windowed = true;
	// Set the scan line ordering and scaling to unspecified.
	m_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Discard the back buffer contents after presenting.
	m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// Don't set the advanced flags.
	m_swapChainDesc.Flags = 0;

	// Set up the depth stencil view description.
	m_depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Set up the description of the depth buffer.
	m_depthBufferDesc.Width = 800;
	m_depthBufferDesc.Height = 600;
	m_depthBufferDesc.MipLevels = 1;
	m_depthBufferDesc.ArraySize = 1;
	m_depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_depthBufferDesc.SampleDesc.Count = 1;
	m_depthBufferDesc.SampleDesc.Quality = 0;
	m_depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_depthBufferDesc.CPUAccessFlags = 0;
	m_depthBufferDesc.MiscFlags = 0;

	// Setup the viewport for rendering.
    m_viewport.Width = 800.f;
    m_viewport.Height = 600.f;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;
}

void BaseDirect::Initialize() {
	// Vytvore zariadenie
	GetGraphicCardInfo();
	SetDirectDesc();
	CreateDevice();
	RegisterDesc();
	
	// Ktore veci chceme pouzivat ?
	CreateRasterizerState();
	CreateDepthStencilState();
	CreateAlphaBlendingState();

	// Defaultne spustacie nastavenie
	TurnZBuffer(true);
	TurnAlphaBlending(false);
	DefaultMatrixes();
}

void BaseDirect::CreateDevice()
{	
	ID3D11Texture2D* backBufferPtr;
	HRESULT hr;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	const D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;
	V( D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &level, 1,  // Set the feature level to DirectX 11.
										   D3D11_SDK_VERSION, &m_swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext) )
	
	// Get the pointer to the back buffer.
	V(  m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr) )

	// Create the render target view with the back buffer pointer.
	V(  m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView) )

	// Release pointer to the back buffer as we no longer need it.
	SAFE_RELEASE( backBufferPtr )
}

void BaseDirect::RegisterDesc()
{	
	HRESULT hr;

	// Create the texture for the depth buffer using the filled out description.
	V( m_device->CreateTexture2D(&m_depthBufferDesc, NULL, &m_depthStencilBuffer) )

	// Create the depth stencil view.
	V( m_device->CreateDepthStencilView(m_depthStencilBuffer, &m_depthStencilViewDesc, &m_depthStencilView) )
	
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Create the viewport.
    m_deviceContext->RSSetViewports(1, &m_viewport);
}

void BaseDirect::FindBestModeForMonitor(IDXGIOutput* adapterOutput, UINT screenWidth, UINT screenHeight) 
{
	DXGI_MODE_DESC* displayModeList;
	UINT numModes, i;
	HRESULT hr;

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	V( adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL) )

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList) { throw std::exception(); }

	// Now fill the display mode list structures.
	V( adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList) )

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(i=0; i<numModes; i++)
	{
		if(displayModeList[i].Width == screenWidth) {
			if(displayModeList[i].Height == screenHeight) {
				m_numerator = displayModeList[i].RefreshRate.Numerator;
				m_denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	SAFE_DELETE_ARRAY( displayModeList)
}

void BaseDirect::GetGraphicCardInfo()
{
	UINT stringLength;	
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	DXGI_ADAPTER_DESC adapterDesc;
	HRESULT hr;

	// Create a DirectX graphics interface factory.
	V( CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory) )

	// Use the factory to create an adapter for the primary graphics interface (video card).
	V( factory->EnumAdapters(0, &adapter) )

	// Enumerate the primary adapter output (monitor).
	V( adapter->EnumOutputs(0, &adapterOutput) )

	// Get the adapter (video card) description.
	V( adapter->GetDesc(&adapterDesc) )

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	int error = wcstombs_s(&stringLength, m_videoCardName, 128, adapterDesc.Description, 128);
	if(error != 0) { throw std::exception(); }
	FindBestModeForMonitor(adapterOutput, 800, 600); 

	// Release everything
	SAFE_RELEASE( adapterOutput )
	SAFE_RELEASE( adapter )
	SAFE_RELEASE( factory )
}

void BaseDirect::DefaultMatrixes() {
	// Setup the projection matrix.
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = 800.f / (float) 600.f;;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(
		&m_projectionMatrix, 
		fieldOfView, 
		screenAspect,
		0.1f, 
		1000.f
	);

    // Initialize the world matrix to the identity matrix.
    D3DXMatrixIdentity(&m_worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&m_orthoMatrix, 
		800.f, 
		600.f, 
		0.1f, 
		1000.f
	);
}

void BaseDirect::BeginScene(float color[4])
{
	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// commands the GPU to render using the current vertex buffer, vertex layout, and primitive topology. 
	// nepouzivat m_deviceContext->Draw(3, 0 );
}

void BaseDirect::CreateDepthStencilState() {
	CreateDepthStencilState(true);
	CreateDepthStencilState(false);
}
void BaseDirect::CreateDepthStencilState(bool zbuffer) {
	// Set up the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = zbuffer;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	

	// Create the depth stencil state.
	HRESULT hr;
	V( m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState[ zbuffer ? 1 : 0 ] ) )
}
void BaseDirect::CreateAlphaBlendingState() {
	CreateAlphaBlendingState(true);
	CreateAlphaBlendingState(false);
}
void BaseDirect::CreateAlphaBlendingState(bool turn) {
	// Clear the blend state description.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = turn;
    blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create hr blend state using the description.
	HRESULT hr;
	V( m_device->CreateBlendState(&blendStateDescription, &m_alphaBlendingState[ turn ? 1 : 0] ) )
}


void BaseDirect::CreateRasterizerState() {
	// Clear the Rasterizer state
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = FALSE; // ciari su hrubsie ale krajsie ... funguje to len ked renderujem ciste ciari
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = FALSE; // ze ci bude na velku dialku rezat trojuholniky
	rasterDesc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_SOLID;	D3D11_FILL_WIREFRAME
	rasterDesc.FrontCounterClockwise = false; // pozor rusi Hud renderovanie
	rasterDesc.MultisampleEnable = TRUE;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	HRESULT hr;
	V( m_device->CreateRasterizerState(&rasterDesc, &m_rasterState) )
	
	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);
}

void BaseDirect::TurnAlphaBlending(bool turn)
{
	// Setup the blend factor.
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaBlendingState[ turn ? 1 : 0 ], blendFactor, 0xffffffff);
}




void BaseDirect::TurnZBuffer(bool x) {
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState[ x ? 1 : 0], 1); // (int) !x		;P
}
ID3D11Device* BaseDirect::GetDevice() {
	return m_device;
}
ID3D11DeviceContext* BaseDirect::GetDeviceContext() {
	return m_deviceContext;
}
D3DXMATRIX* BaseDirect::GetProjectionMatrix() {
	return &m_projectionMatrix;
}
D3DXMATRIX* BaseDirect::GetWorldMatrix() {
	return &m_worldMatrix;
}
D3DXMATRIX* BaseDirect::GetOrthoMatrix() {
	return &m_orthoMatrix;
}

int BaseDirect::GetMemorySize() {
	return m_videoCardMemory;
}
const char* BaseDirect::GetVideoCardName() {
	return m_videoCardName;
}
