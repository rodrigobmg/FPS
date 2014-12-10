#ifndef _SHADERCLASS_H_
#define _SHADERCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "../../../Global.h"
using namespace std;

class Shader
{
public:
	Shader();
	~Shader();

protected:
	/*
	string filename;
	virtual const WCHAR* getName(); // zada meno Light a hotovo
	virtual LPCSTR getVertexFunctionName(); // ak je meno funkcie ine
	virtual LPCSTR getPixelFunctionName(); // ak je meno funkcie ine
	virtual const WCHAR* getVertexFileName(); // ak ten subor ma inu koncovku
	virtual const WCHAR* getPixelFileName(); // ak ten subor ma inu koncovku
	virtual UINT getPolygonLayoutNum();
	virtual D3D11_INPUT_ELEMENT_DESC* getPolygonLayout();
	virtual void PostInit(D3D11_MAPPED_SUBRESOURCE *data); // este niecospravit ?
	*/

	
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
	
	void InitBase(ID3D11Device*, wstring&, string function, D3D11_INPUT_ELEMENT_DESC *, UINT numElements);
	ID3D10Blob* Compile(const WCHAR* vsFilename, LPCSTR pFunctionName, LPCSTR pProfile); 
	void GetShaderBuffersBySimpleName(wstring &name, string &function, ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage,const  WCHAR* shaderFilename);
	void SetShaderBaseParameters(ID3D11DeviceContext*, D3DXMATRIX &, D3DXMATRIX &, D3DXMATRIX &, ID3D11ShaderResourceView*, D3D11_MAPPED_SUBRESOURCE*);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

inline Shader::Shader() {
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}
inline Shader::~Shader() {
	// Shutdown the vertex and pixel shaders as well as the related objects.
	SAFE_RELEASE( m_sampleState )
	SAFE_RELEASE( m_matrixBuffer )
	SAFE_RELEASE( m_layout )
	SAFE_RELEASE( m_pixelShader )
	SAFE_RELEASE( m_vertexShader )
}
inline ID3D10Blob* Shader::Compile(const WCHAR* vsFilename, LPCSTR pFunctionName, LPCSTR pProfile) {  
	// Compile the vertex shader code.
	ID3D10Blob* errorMessage = 0;
	ID3D10Blob* ShaderBuffer = 0;
	if(FAILED(		D3DX11CompileFromFile(
					vsFilename, NULL, NULL, 
					pFunctionName, pProfile, 
					D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
					&ShaderBuffer, &errorMessage, NULL) )
	) {
		// If the shader failed to compile it should have writen something to the error message.
		
		if(errorMessage) {
			OutputShaderErrorMessage(errorMessage, vsFilename);
		} else {
			// If there was nothing in the error message then it simply could not find the shader file itself.
			throw new exception("Missing Shader File");
		}
	}
	return ShaderBuffer;
}
inline void Shader::GetShaderBuffersBySimpleName(wstring &name, string &function, ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer) {
	wstring filename[2];
	filename[0].append(name).append(wstring(L".vs"));
	filename[1].append(name).append(wstring(L".ps"));
	
	string functionname[2];
	functionname[0].append(function).append("VertexShader");
	functionname[1].append(function).append("PixelShader");

	*vertexShaderBuffer = Compile(filename[0].c_str(), functionname[0].c_str(), "vs_5_0");
	*pixelShaderBuffer = Compile(filename[1].c_str(), functionname[1].c_str(), "ps_5_0");
}

inline void Shader::InitBase(
	ID3D11Device* device, 
	wstring &name, 
	string function,
	D3D11_INPUT_ELEMENT_DESC *polygonLayout, 
	UINT numElements
) {
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	GetShaderBuffersBySimpleName(name, function, &vertexShaderBuffer, &pixelShaderBuffer);

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result)) {
		throw new exception("CreateVertexShader error.");
	}

    // Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result)) {
		throw new exception("CreatePixelShader error.");
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(
		polygonLayout, numElements, 
		vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), 
		&m_layout);
	if(FAILED(result)) {
		throw new exception("CreateInputLayout error.");
	}
	
	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	SAFE_RELEASE ( vertexShaderBuffer ) 
	SAFE_RELEASE ( pixelShaderBuffer )

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
    result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result)) {
		throw new exception("Error CreateSamplerState.");
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result)) {
		throw new exception("CreateBuffer m_matrixBuffer error.");
	}
}
inline void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	fout << shaderFilename;
	fout << endl;
	for(i=0; i<bufferSize; i++) {
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	SAFE_RELEASE( errorMessage )

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	throw new exception("Error compiling shader.  Check shader-error.txt for message.");
}
inline void Shader::SetShaderBaseParameters(
	ID3D11DeviceContext* deviceContext, 
	D3DXMATRIX &worldMatrix, 
	D3DXMATRIX &viewMatrix, 
	D3DXMATRIX &projectionMatrix, 
	ID3D11ShaderResourceView* texture,
	D3D11_MAPPED_SUBRESOURCE *mappedResource
) {
	HRESULT result;
	MatrixBufferType* dataPtr;
	UINT bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, mappedResource);
	if(FAILED(result)) {
		throw new exception("Error, cant lock the constant buffer.");
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource->pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
    deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
}
inline void Shader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

    // Set the vertex and pixel shaders that will be used to render this triangle.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

#endif