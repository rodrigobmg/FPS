#include "class.HLSL.h"
#include <fstream>
#include <assert.h>
#include "../../../Global.h"

using namespace Shaders;
using namespace std;

HLSLBase::HLSLBase() {
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_sampleState = 0;
	m_layout = 0;
}

HLSLBase::~HLSLBase() {
	// Shutdown the vertex and pixel Bases as well as the related objects.
	SAFE_RELEASE( m_layout )
	SAFE_RELEASE( m_pixelShader )
	SAFE_RELEASE( m_vertexShader )
	SAFE_RELEASE( m_sampleState )
}

void HLSLBase::InitializeBase(
	ID3D10Blob* vertexBaseBuffer, 
	ID3D10Blob* pixelBaseBuffer,
	D3D11_INPUT_ELEMENT_DESC* polygonLayout,
	DWORD num
) {
	ID3D11Device* device = Direct()->GetDevice();
	HRESULT result;

	// Create the vertex HLSLBase from the buffer.
	result = device->CreateVertexShader(vertexBaseBuffer->GetBufferPointer(), vertexBaseBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result)) {
		throw new exception("CreateVertexBase error.");
	}

    // Create the pixel HLSLBase from the buffer.
    result = device->CreatePixelShader(pixelBaseBuffer->GetBufferPointer(), pixelBaseBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result)) {
		throw new exception("CreatePixelBase error.");
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(
		polygonLayout, num, 
		vertexBaseBuffer->GetBufferPointer(), 
		vertexBaseBuffer->GetBufferSize(), 
		&m_layout
	);
	if(FAILED(result)) {
		throw new exception("CreateInputLayout error.");
	}
	
	// Release the vertex HLSLBase buffer and pixel HLSLBase buffer since they are no longer needed.
	SAFE_RELEASE ( vertexBaseBuffer ) 
	SAFE_RELEASE ( pixelBaseBuffer )
}

void HLSLBase::Initialize(
	wstring name, 
	string function,
	D3D11_INPUT_ELEMENT_DESC* polygonLayout,
	DWORD num
) {
	ID3D10Blob* vertexBaseBuffer;
	ID3D10Blob* pixelBaseBuffer;
	GetBuffersBySimpleName(name, function, &vertexBaseBuffer, &pixelBaseBuffer);

	InitializeBase(
		vertexBaseBuffer, 
		pixelBaseBuffer,
		polygonLayout,
		num
	);
}

void HLSLBase::GetBuffersBySimpleName(wstring &name, string &function, ID3D10Blob** vertexBaseBuffer, ID3D10Blob** pixelBaseBuffer) {
	wstring filename[2];
	filename[0].append(name).append(wstring(L".vs"));
	filename[1].append(name).append(wstring(L".ps"));
	
	string functionname[2];
	functionname[0].append(function).append("VertexShader");
	functionname[1].append(function).append("PixelShader");

	*vertexBaseBuffer = Compile(filename[0].c_str(), functionname[0].c_str(), "vs_5_0");
	*pixelBaseBuffer = Compile(filename[1].c_str(), functionname[1].c_str(), "ps_5_0");
	assert(*vertexBaseBuffer);
	assert(*pixelBaseBuffer);
}

void HLSLBase::OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* BaseFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("HLSLBase-error.txt");

	// Write out the error message.
	fout << BaseFilename;
	fout << endl;
	for(i=0; i<bufferSize; i++) {
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	SAFE_RELEASE( errorMessage )
}

void HLSLBase::CreateSamplerState() 
{
	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	assert(m_sampleState == 0);
	if(FAILED(Direct()->GetDevice()->CreateSamplerState(&samplerDesc, &m_sampleState) )) {
		throw new exception("Error CreateSamplerState.");
	}
}
ID3D10Blob* HLSLBase::Compile(const WCHAR* vsFilename, LPCSTR pFunctionName, LPCSTR pProfile) {  
	// Compile the vertex HLSLBase code.
	ID3D10Blob* errorMessage = 0;
	ID3D10Blob* BaseBuffer = 0;
	if(FAILED(		D3DX11CompileFromFile(
					vsFilename, NULL, NULL, 
					pFunctionName, pProfile, 
					D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
					&BaseBuffer, &errorMessage, NULL) )
	) {
		// If the HLSLBase failed to compile it should have writen something to the error message.
		
		if(errorMessage) {
			OutputShaderErrorMessage(errorMessage, vsFilename);
			throw new exception("Cant compile shader file.");
		} else {
			// If there was nothing in the error message then it simply could not find the HLSLBase file itself.
			throw new exception("Missing shader file.");
		}
	}
	return BaseBuffer;
}

void HLSLBase::Render()
{
	ID3D11DeviceContext* deviceContext = Direct()->GetDeviceContext();
	
	// Set the vertex input layout.
	assert(m_layout);
	deviceContext->IASetInputLayout(m_layout);

    // Set the vertex and pixel shader that will be used to render this triangle.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	
	// Set the sampler state in the pixel HLSLBase.
	if(m_sampleState != NULL) {
		deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	}
}
