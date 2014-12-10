#pragma once
#include "DirectFPS.h"
#include "../Help/App/class.Direct.h"
#include <exception>

#include "../Help/Graphic/class.MoveableCamera.h"
#include "../Help/Graphic/Base/class.Model.h"
#include "../Help/Graphic/Shader/Shaders.h"
#include "../Help/Graphic/Base/class.Light.h"
#include "../Help/Graphic/Base/class.Text.h"
#include "../Help/Graphic/Base/class.Modellist.h"
#include "../Help/Shapes/Shapes.h"
#include "../Help/Map/Terrain.h"
#include "../Help/Graphic/Base/class.Model.h"
#include "../Help/Graphic/Shader/Shaders.h"
#include "../Help/Graphic/Base/class.Light.h"
#include "../Help/Base/QuickDepthSort.h"
using namespace Shape;

extern Direct* gDirect;
extern Game* gGame;
CModelViewerCamera g_Camera;

extern HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, ID3D11DeviceContext* devcontext, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext );

extern void CALLBACK OnD3D11DestroyDevice(void* pUserContext );
extern CTerrain g_Terrain;
extern void CALLBACK OnD3D11FrameRender(Direct* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );



struct Player {
	vec pos;
	vec dir;
};

Player g_pPlayers;
CTerrain                        g_Terrain;
UINT                            g_NumThreads = 0;
bool                            g_bWireframe = true;
bool                            g_bShowTiles = true;
bool                            g_bRenderBalls = true;
bool                            g_bRunSim = true;
bool                            g_bBallCam = false;
bool                            g_bRenderGrass = true;
bool                            g_bEnableCulling = false;
bool                            g_bDepthCullGrass = true;
float                           g_fFOV = 80.0f * ( D3DX_PI / 180.0f );
CGrowableArray <UINT>           g_VisibleTileArray;
CGrowableArray <float>          g_VisibleTileArrayDepth;
float                           g_fWorldScale = 400.0f;
float                           g_fHeightScale = 40.0f;
UINT                            g_SqrtNumTiles = 40;
UINT                            g_SidesPerTile = 20;
float                           g_fFadeStart = 40.0f;
float                           g_fFadeEnd = 60.0f;
UINT                            g_NumVisibleBalls = 0;
UINT                            g_NumGrassTiles = 0;
bool                            g_bUseFloat32Textures = false;

extern ID3D11Buffer*            g_pStreamDataVB10;
extern ID3D11Buffer*            g_pGrassDataVB10;

void GetCameraData( D3DXMATRIX* pmWorld, D3DXMATRIX* pmView, D3DXMATRIX* pmProj, D3DXVECTOR3* pvEye,
                    D3DXVECTOR3* pvDir )
{
    D3DXVECTOR3 pos = *g_Camera.GetEyePt();
    *pvEye = pos + D3DXVECTOR3( 0, 2, 0 );
    *pvDir = *g_Camera.GetLookAtPt();
    *pvEye -= ( *pvDir ) * 5.0f;
    *pmProj = *g_Camera.GetProjMatrix();
    D3DXVec3Normalize( pvDir, pvDir );

    if( g_bBallCam )
    {
        float fHeight = g_Terrain.GetHeightOnMap( pvEye );
        if( fHeight > ( pvEye->y - 1.5f ) )
        {
            pvEye->y = fHeight + 1.5f;
        }

        D3DXVECTOR3 vAt = pos + D3DXVECTOR3( 0, 2, 0 );
        D3DXVECTOR3 vUp( 0,1,0 );
        D3DXMatrixIdentity( pmWorld );
        D3DXMatrixLookAtLH( pmView, pvEye, &vAt, &vUp );
    }
    else
    {
        *pmWorld = *g_Camera.GetWorldMatrix();
        *pmView = *g_Camera.GetViewMatrix();
    }
}
void GetCameraCullPlanes( D3DXVECTOR3* p1Normal, D3DXVECTOR3* p2Normal, float* p1D, float* p2D )
{
    D3DXVECTOR3 vEye;
    D3DXVECTOR3 vDir;
    D3DXMATRIX mCamWorld;
    D3DXMATRIX mView;
    D3DXMATRIX mProj;
    GetCameraData( &mCamWorld, &mView, &mProj, &vEye, &vDir );

    // setup clip planes
    D3DXVECTOR3 vLeftNormal;
    D3DXVECTOR3 vRightNormal;
    D3DXMATRIX mRotLeft;
    D3DXMATRIX mRotRight;
    float fAngle = D3DX_PI / 2.0f + ( g_fFOV / 2.0f ) * 1.3333f;
    D3DXMatrixRotationY( &mRotLeft, -fAngle );
    D3DXMatrixRotationY( &mRotRight, fAngle );
    D3DXVec3TransformNormal( &vLeftNormal, &vDir, &mRotLeft );
    D3DXVec3TransformNormal( &vRightNormal, &vDir, &mRotRight );
    *p1D = D3DXVec3Dot( &vLeftNormal, &vEye );
    *p2D = D3DXVec3Dot( &vRightNormal, &vEye );
    *p1Normal = vLeftNormal;
    *p2Normal = vRightNormal;
}
void VisibilityCullTiles(ID3D11DeviceContext* con)
{
    g_VisibleTileArray.Reset();
    g_VisibleTileArrayDepth.Reset();

    D3DXVECTOR3 vEye;
    D3DXVECTOR3 vDir;
    D3DXMATRIX mCamWorld;
    D3DXMATRIX mView;
    D3DXMATRIX mProj;
    GetCameraData( &mCamWorld, &mView, &mProj, &vEye, &vDir );

    // setup clip planes
    D3DXVECTOR3 vLeftNormal;
    D3DXVECTOR3 vRightNormal;
    float leftD;
    float rightD;
    GetCameraCullPlanes( &vLeftNormal, &vRightNormal, &leftD, &rightD );

    for( UINT i = 0; i < g_Terrain.GetNumTiles(); i++ )
    {
        TERRAIN_TILE* pTile = g_Terrain.GetTile( i );

        if( g_bEnableCulling )
        {
            D3DXVECTOR3 c1( pTile->BBox.min.x, 0, pTile->BBox.min.z );
            D3DXVECTOR3 c2( pTile->BBox.max.x, 0, pTile->BBox.max.z );
            D3DXVECTOR3 c3( pTile->BBox.min.x, 0, pTile->BBox.max.z );
            D3DXVECTOR3 c4( pTile->BBox.max.x, 0, pTile->BBox.min.z );

            if( D3DXVec3Dot( &c1, &vLeftNormal ) > leftD &&
                D3DXVec3Dot( &c2, &vLeftNormal ) > leftD &&
                D3DXVec3Dot( &c3, &vLeftNormal ) > leftD &&
                D3DXVec3Dot( &c4, &vLeftNormal ) > leftD )
                continue;
            if( D3DXVec3Dot( &c1, &vRightNormal ) > rightD &&
                D3DXVec3Dot( &c2, &vRightNormal ) > rightD &&
                D3DXVec3Dot( &c3, &vRightNormal ) > rightD &&
                D3DXVec3Dot( &c4, &vRightNormal ) > rightD )
                continue;
        }

        D3DXVECTOR3 middle = ( pTile->BBox.min + pTile->BBox.max ) / 2.0f;
        D3DXVECTOR3 toTile = middle - vEye;
        float fDot = D3DXVec3Dot( &toTile, &vDir );
        float fDist = D3DXVec3Length( &toTile );
        if( fDot < 0 )
            fDist = fDot;
        g_VisibleTileArray.Add( i );
        g_VisibleTileArrayDepth.Add( fDist );
    }

    UINT* pTiles = g_VisibleTileArray.GetData();
    float* pDepth = g_VisibleTileArrayDepth.GetData();

    QuickDepthSort( pTiles, pDepth, 0, g_VisibleTileArray.GetSize() - 1 );

    // Fill the grass instance VB
    HRESULT hr = S_OK;
    g_NumGrassTiles = 0;
    float fTileRad = ( g_fWorldScale / g_SqrtNumTiles ) / 2.0f;
    D3DXVECTOR3* pPositions = NULL;

    //V( g_pGrassDataVB10->Map( D3D11_MAP_WRITE_DISCARD, 0, ( void** )&pPositions ) );
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3DXVECTOR3* dataPtr;
	con->Map(g_pGrassDataVB10, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (D3DXVECTOR3*) mappedResource.pData;

    UINT TileCount = g_VisibleTileArray.GetSize();
    for( int i = TileCount - 1; i >= 0; i-- )
    {
        TERRAIN_TILE* pTile = g_Terrain.GetTile( g_VisibleTileArray.GetAt( i ) );
        D3DXVECTOR3 middle = ( pTile->BBox.min + pTile->BBox.max ) / 2.0f;

        //-----------------------------------------------------------------------------------------
        // o/__   <-- BreakdancinBob TODO: We're just drawing all of the grass tiles regardless of  
        // |  (\						   whether we can actually see them.  Grass drawing is
        //								   instanced, so it's not a strain on draw calls.  However,
        //                                 the amount of grass we're actually drawing is a strain
        //                                 on the GPU itself.
        //                          
        //                           HINT: Let's cull not draw the grass if it's going to be too
        //								   far away to be visible.
        //-----------------------------------------------------------------------------------------

        //-----------------------------------------------------------------------------------------
        //	Answer
        D3DXVECTOR3 vToTile = middle - vEye;
        float fDist = D3DXVec3Length( &vToTile );

        if( !g_bDepthCullGrass ||
            ( fDist - fTileRad < g_fFadeEnd ) )
        {
            //pPositions[ g_NumGrassTiles ] = middle;
			dataPtr[ g_NumGrassTiles ] = middle;
            g_NumGrassTiles ++;
        }
        //-----------------------------------------------------------------------------------------

    }
	 con->Unmap(g_pGrassDataVB10, 0);
    //g_pGrassDataVB10->Unmap();
}



class Tutorial : public Module {
public:	
	Model* m_Model;
	LightShader* m_LightShader;
	Light* m_Light;

	Tutorial() {
		g_Camera.SetViewParams(&vec(0.0f, 0.0f, 0.0f), &vec(0.0f, 0.0f, -10.0f));

		// Create the model object.
		m_Model = new Model;

		// Initialize the model object.
		HRESULT result = m_Model->Initialize(gDirect->GetDevice(), "../Media/cube.txt", L"../Media/seafloor.dds");
		if(!result) {
			throw new std::exception("Could not initialize the model object.");
		}

		// Initialize the light shader object.
		m_LightShader = new LightShader;
		m_LightShader->Initialize(gDirect->GetDevice(), L"../Media/light");

		// Initialize the light object.
		m_Light = new Light;
		m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetDirection(0.0f, 0.0f, 1.0f);

		OnD3D11CreateDevice(gDirect->GetDevice(), gDirect->GetDeviceContext(), NULL, NULL);
		g_Terrain.OnCreateDevice(gDirect->GetDevice(), gDirect->GetDeviceContext());
		gGame->Add(gInput);
	}
	~Tutorial() {
		OnD3D11DestroyDevice(NULL);
		g_Terrain.OnDestroyDevice();
	}

	virtual void Frame() {
		// Generate the view matrix based on the camera's position.
		D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, *orthoMatrix;

		// Get the view, projection, and world matrices from the camera and D3D objects.
		viewMatrix = *g_Camera.GetViewMatrix(); //->GetViewMatrix();
		worldMatrix = *g_Camera.GetWorldMatrix(); //gDirect->GetWorldMatrix();
		projectionMatrix = *g_Camera.GetProjMatrix(); // gDirect->GetProjectionMatrix();
		orthoMatrix = gDirect->GetOrthoMatrix();

		OnD3D11FrameRender(gDirect, gGame->GetGlobalTime(), gGame->GetLeftTime(), NULL);

		// Update the camera's position based on user input 
		g_Camera.FrameMove( gGame->GetLeftTime() );

		// Rotate the world matrix by the rotation value so that the triangle will spin.
		static float rotation = 0.0f;
		// Update the rotation variable each frame.
		rotation += (float)D3DX_PI * 0.01f;
		if(rotation > 360.0f) {
			rotation -= 360.0f;
		}
		//D3DXMatrixRotationY(&worldMatrix, rotation);
		//D3DXMatrixTranslation(&worldMatrix, 0.0f, 0.0f, 0.f);

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Model->Render(gDirect->GetDeviceContext());

		// Render the model using the color shader.
		m_LightShader->Render(gDirect->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
							m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	}
};