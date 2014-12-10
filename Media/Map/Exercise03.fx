//--------------------------------------------------------------------------------------
// Exercise03.fx
// PIX Workshop GDC 2007
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// o/__   <-- Breakdancin' Bob will guide you through the exercise
// |  (\    
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VSBasicIn
{
	float4 Pos						: POSITION;
	float3 Norm						: NORMAL;
	float2 Tex						: TEXCOORD0;
};

struct VSBallIn
{
	float4 Pos						: POSITION;
	float3 Norm						: NORMAL;
	float2 Tex						: TEXCOORD0;
	float3 BallPosition				: TEXCOORD1;
};

struct VSGrassIn
{
	float4 Pos						: POSITION;
	float2 Tex						: TEXCOORD0;
	float3 GrassPosition			: TEXCOORD1;
};

struct PSBasicIn
{
	float4 Pos						: POSITION;
	float3 Norm						: TEXCOORD0;
	float2 Tex						: TEXCOORD1;
};

struct PSGrassIn
{
	float4 Pos						: POSITION;
	float2 Tex						: TEXCOORD0;
	float  Alpha					: TEXCOORD1;
	float  Lighting					: TEXCOORD2;
};

//-----------------------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------------------
matrix g_mWorldViewProj;
matrix g_mWorld;
float3 g_vWorldLightDir;
float  g_fTime;
float  g_fElapsedTime;
float4 g_vColor;
float  g_fWorldScale;
float  g_fHeightScale;
float3 g_vEyePt;
float  g_fFadeStart;
float  g_fFadeEnd;


//-----------------------------------------------------------------------------------------
// Textures and Samplers
//-----------------------------------------------------------------------------------------
Texture2D		g_txDiffuse;
Texture2D		g_txNormal;
Texture2D		g_txHeight;
Texture2D		g_txDirt;
Texture2D		g_txGrass;
Texture2D		g_txMask;
Texture2D		g_txShadeNormals;

SamplerState DiffuseSampler
{
    Texture = (g_txDiffuse);
	Filter = MIN_MAG_MIP_LINEAR;

    AddressU = WRAP;
    AddressV = WRAP;
};
SamplerState DiffuseSamplerClamp
{
    Texture = (g_txDiffuse);
	Filter = MIN_MAG_MIP_LINEAR;

    AddressU = CLAMP;
    AddressV = CLAMP;
};
SamplerState NormalSampler
{
    Texture = (g_txNormal);
	Filter = MIN_MAG_MIP_LINEAR;

    AddressU = WRAP;
    AddressV = WRAP;
};
SamplerState HeightSampler
{
    Texture = (g_txHeight);
	Filter = MIN_MAG_LINEAR_MIP_POINT;

    AddressU = WRAP;
    AddressV = WRAP;
};
SamplerState DirtSampler
{
    Texture = (g_txDirt);
	Filter = MIN_MAG_MIP_LINEAR;

    AddressU = WRAP;
    AddressV = WRAP;
};
SamplerState GrassSampler
{
    Texture = (g_txGrass);
	Filter = MIN_MAG_MIP_LINEAR;

    AddressU = WRAP;
    AddressV = WRAP;
};
SamplerState MaskSampler
{
    Texture = (g_txMask);
	Filter = MIN_MAG_LINEAR_MIP_POINT;

    AddressU = WRAP;
    AddressV = WRAP;
};
SamplerState ShadeNormalsSampler
{
    Texture = (g_txShadeNormals);
	Filter = MIN_MAG_LINEAR_MIP_POINT;

    AddressU = WRAP;
    AddressV = WRAP;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSBasic
//-----------------------------------------------------------------------------------------
PSBasicIn VSBasic(VSBasicIn input)
{
	PSBasicIn output;
	
	output.Pos = mul( input.Pos, g_mWorldViewProj );
	output.Norm = normalize( mul( input.Norm, (float3x3)g_mWorld ) );
	output.Tex = input.Tex;

	return output;
}


//-----------------------------------------------------------------------------------------
// VertexShader: VSInstancedBall
//-----------------------------------------------------------------------------------------
PSBasicIn VSInstancedBall(VSBallIn input)
{
	PSBasicIn output;
	
	float4 pos = input.Pos + float4(input.BallPosition,0);
	output.Pos = mul( pos, g_mWorldViewProj );
	output.Norm = normalize( mul( input.Norm, (float3x3)g_mWorld ) );
	output.Tex = input.Tex;

	return output;
}


//-----------------------------------------------------------------------------------------
// VertexShader: VSGrass
//-----------------------------------------------------------------------------------------
PSGrassIn VSGrass(VSGrassIn input)
{
	PSGrassIn output;
	
	float4 pos = input.Pos + float4(input.GrassPosition,0);
	float2 tex = float2(pos.x, pos.z);
	tex.x = (tex.x / g_fWorldScale) + 0.5;
	tex.y = 1.0 - ( (tex.y / g_fWorldScale) + 0.5 );
	
	float height = g_txHeight.SampleLevel( HeightSampler, tex, 0);
	float mask = ceil( g_txMask.SampleLevel( MaskSampler, tex, 0) - 0.3 );
	float3 normal = g_txShadeNormals.SampleLevel( ShadeNormalsSampler, tex, 0);
	normal = normal*2 - 1;
	normal.z = -normal.z;
	float lighting = max( 0.3, saturate( dot( normal, g_vWorldLightDir ) ) );
	
	pos.y = pos.y + height*g_fHeightScale;
	
	output.Pos = mul( pos, g_mWorldViewProj );
	output.Tex = input.Tex;
	output.Alpha = 1 - max( length( pos - g_vEyePt ) - g_fFadeStart, 0 ) / (g_fFadeEnd-g_fFadeStart);
	output.Alpha *= mask;
	output.Lighting = lighting;
	return output;
}


//-----------------------------------------------------------------------------------------
// PixelShader: PSTerrain
//-----------------------------------------------------------------------------------------
float4 PSTerrain(PSBasicIn input) : SV_Target
{
	// In SM4.0 (D3D10 HLSL) you no longer use the tex*D intrinsic to sample a texture. Textures are objects (kinda like a C++ object) and have methods that you call to sample 
	float mask = g_txMask.Sample(MaskSampler, input.Tex);// tex2D( MaskSampler, input.Tex );
	float4 dirt = g_txDirt.Sample(DirtSampler, input.Tex); // tex2D( DirtSampler, input.Tex );
	float4 grass = g_txGrass.Sample(GrassSampler, input.Tex); //tex2D( GrassSampler, input.Tex );
	
	float4 diffuse = lerp( dirt, grass, mask );
	float3 bump = g_txNormal.Sample( NormalSampler, input.Tex*100.0f ).xzy;
	bump = bump*2 - 1;
	bump.z = -bump.z;
	
	float3 normal = normalize( input.Norm + bump*0.5 );
	float lighting = max( 0.3, saturate( dot( normal, g_vWorldLightDir ) ) );
	return diffuse * lighting * g_vColor;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSBall
//-----------------------------------------------------------------------------------------
float4 PSBall(PSBasicIn input) : SV_Target
{
	float4 diffuse = float4(1,0,0,1);
	
	float3 normal = input.Norm;
	float lighting = max( 0.3, saturate( dot( normal, g_vWorldLightDir ) ) );
	return lighting*diffuse;
}


//-----------------------------------------------------------------------------------------
// PixelShader: PSGrass
//-----------------------------------------------------------------------------------------
float4 PSGrass(PSGrassIn input) : SV_Target
{	
	float4 diffuse = g_txDiffuse.Sample( DiffuseSamplerClamp, input.Tex );
	diffuse.xyz *= input.Lighting;
	diffuse.a *= input.Alpha;
	return diffuse;
}


//-----------------------------------------------------------------------------------------
// PixelShader: PSSky
//-----------------------------------------------------------------------------------------
float4 PSSky(PSBasicIn input) : SV_Target
{	
	//return float4( input.Tex, 0, 1 );
	//float2 tex = input.Tex * float2(0.5,0.5);
	//tex.y += 0.5;
	float4 diffuse = g_txDiffuse.Sample( DiffuseSampler, input.Tex );
	return diffuse;
}


//-----------------------------------------------------------------------------------------
// Technique: RenderTerrain
//-----------------------------------------------------------------------------------------
technique11 RenderTerrain
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSBasic() ) );
        SetPixelShader( CompileShader( ps_4_0, PSTerrain() ) );
    }
}

//-----------------------------------------------------------------------------------------
// Technique: RenderBall
//-----------------------------------------------------------------------------------------
technique RenderBall
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSInstancedBall() ) );
        SetPixelShader( CompileShader( ps_4_0, PSBall() ) );		
    }  
}


//-----------------------------------------------------------------------------------------
// Technique: RenderGrass
//-----------------------------------------------------------------------------------------
technique RenderGrass
{
    pass p0
    {
		SetVertexShader( CompileShader( vs_4_0, VSGrass() ) );
        SetPixelShader( CompileShader( ps_4_0, PSGrass() ) );
          
        ZWriteEnable = FALSE;
        CullMode = NONE; 
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA; 
    }  
}


//-----------------------------------------------------------------------------------------
// Technique: RenderSky
//-----------------------------------------------------------------------------------------
technique RenderSky
{
    pass p0
    {
		SetVertexShader( CompileShader( vs_4_0, VSBasic() ) );
        SetPixelShader( CompileShader( ps_4_0, PSSky() ) );
        
        ZWriteEnable = FALSE;
        ZEnable = FALSE;
    }  
}


BlendState AdditiveBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState NoBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState DisableDepthWrite
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

RasterizerState CullBack
{
	CullMode = BACK;
};

RasterizerState CullNone
{
	CullMode = NONE;
};

//-----------------------------------------------------------------------------------------
// Technique: RenderTerrain
//-----------------------------------------------------------------------------------------
technique10 RenderTerrain10
{
    pass p0
    {
		SetVertexShader( CompileShader( vs_4_0, VSBasic() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PSTerrain() ) ); 
		
		SetDepthStencilState( EnableDepth, 0 );
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetRasterizerState( CullBack );  
    }  
}


//-----------------------------------------------------------------------------------------
// Technique: RenderBall
//-----------------------------------------------------------------------------------------
technique10 RenderBall10
{
    pass p0
    {
		SetVertexShader( CompileShader( vs_4_0, VSInstancedBall() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PSBall() ) );    
		
		SetDepthStencilState( EnableDepth, 0 );
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetRasterizerState( CullBack );  
    }  
}


//-----------------------------------------------------------------------------------------
// Technique: RenderGrass
//-----------------------------------------------------------------------------------------
technique10 RenderGrass10
{
    pass p0
    {
		SetVertexShader( CompileShader( vs_4_0, VSGrass() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PSGrass() ) );   
        
        SetDepthStencilState( DisableDepthWrite, 0 );
        SetBlendState( AdditiveBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetRasterizerState( CullNone ); 
    }  
}


//-----------------------------------------------------------------------------------------
// Technique: RenderSky
//-----------------------------------------------------------------------------------------
technique10 RenderSky10
{
    pass p0
    {
		SetVertexShader( CompileShader( vs_4_0, VSBasic() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PSSky() ) );   
        
        SetDepthStencilState( DisableDepth, 0 );
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetRasterizerState( CullBack );  
    }  
}