#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

#include "struct.MatrixList.h"
#include "class.Buffers.h"
#include "../Inter/class.IGraphicAutoDevice.h"

namespace Shaders
{

/*
	// Definicia = Renderuj objekt teda strukturu roznymy vlastnostamy teda shaderamy a pouzi rozne udaje teda Features	
	// TODO: No, just cashe your layouts and reuse them in the next frame. 
	// TODO: BufferMaxtrixPrepare pouzit template
*/


// AK staticke tak HLSL
// A odvadza sa od HLSLHelp

class HLSLBase :
	public IGraphicAutoDevice
{
public:		
	HLSLBase();
	~HLSLBase();
	
protected:
	// Inicializuj shader, vytvor vsetke veci ake potrebujes
	void Initialize( 
		std::wstring name, 
		std::string function,
		D3D11_INPUT_ELEMENT_DESC* polygonLayout,
		DWORD num
	);
	void CreateSamplerState();
	void Render();

private:
	// Premenne
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout; // tiez staticke input layout ?
	ID3D11SamplerState* m_sampleState;
	
	// Mini pomocky pre init
	ID3D10Blob* Compile(const WCHAR* vsFilename, LPCSTR pFunctionName, LPCSTR pProfile); 
	void GetBuffersBySimpleName(std::wstring &name, std::string &function, ID3D10Blob** vertexBaseBuffer, ID3D10Blob** pixelBaseBuffer);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const  WCHAR* BaseFilename);

	void InitializeBase(
		ID3D10Blob* vertexBaseBuffer, 
		ID3D10Blob* pixelBaseBuffer,
		D3D11_INPUT_ELEMENT_DESC* polygonLayout,
		DWORD num
	);
};


/*
	 Pozor pri posielani features do shaderov zalezina poradi !
*/
template<typename TYPE>
class HLSL : public HLSLBase, protected BuffersAssistant {
public:
	TYPE* Features() { return &m_features; }

protected:
	TYPE m_features;
};



}


