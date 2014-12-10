#include "Shaders.h"
using namespace Shaders;
using namespace Texture;

void Shader::Render()
{	
	// Set the shader parameters that it will use for rendering.
	Buffer()->PrepareMatrix(&(Features()->matrix));
	Buffer()->Restart();

	Features()->texture.Render();
	HLSLBase::Render();
}

void Shader::Initialize()
{
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Nacitaj veci
	HLSLBase::Initialize(L"../Media/texture", "Texture", polygonLayout, 2);
	HLSLBase::CreateSamplerState();
	Buffer()->Create(1);
	Buffer()->Add(sizeof(MatrixList));
}