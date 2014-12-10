#include "Shaders.h"
using namespace Shaders;
using namespace Light;

void Shader::Render()
{	
	// Set the shader parameters that it will use for rendering.
	Buffer()->PrepareMatrix(&(Features()->matrix));
	Buffer()->Prepare(&(Features()->light), true);
	Buffer()->Restart();

	// Now render the prepared buffers with the shader.
	Features()->texture.Render();
	HLSLBase::Render();
}

void Shader::Initialize()
{
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Nacitaj veci
	HLSLBase::Initialize(L"../Media/Shaders/light", "Light", polygonLayout, 3);
	HLSLBase::CreateSamplerState(); // TODO: toto asi netreba ... pre etxtury ?
	Buffer()->Create(2);
	Buffer()->Add(sizeof(MatrixList));
	Buffer()->Add(sizeof(Features::Light));
}


	/*
		Buffers* a = new Buffers;
		a->Add(&(Features()->matrix), sizeof(MatrixList), SHADERTYPE::VS);
		a->Add(&(Features()->light), sizeof(LightFeatures::Light), SHADERTYPE::PS);
		Add( a );
		Add( &(Features()->textures) );

		// A potom nemusime mat ani Render a vsetko sa spravy samo po Initialize
		void Add( IShader );
				enum SHADERTYPE {
			VS = 0;
			PS = 1;
		}

		struct BufferItem {
			void *pData;
			size_t size;
			SHADERTYPE shader; // VS alebo PS

		};


	*/