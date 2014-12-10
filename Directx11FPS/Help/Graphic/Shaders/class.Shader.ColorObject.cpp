#include "Shaders.h"
using namespace Shaders;
using namespace ColorObject;

void Shader::Render()
{	
	// Set the shader parameters that it will use for rendering.
	Buffer()->PrepareMatrix(&(Features()->matrix));
	Buffer()->Prepare(&(Features()->color), true);
	Buffer()->Restart();
	HLSLBase::Render();
}

void Shader::Initialize()
{
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	// Nacitaj veci
	HLSLBase::Initialize(L"../Media/Shaders/colorobject", "ColorObject", polygonLayout, 1);
	Buffer()->Create(2);
	Buffer()->Add(sizeof(MatrixList));
	Buffer()->Add(sizeof(D3DXVECTOR4));
}