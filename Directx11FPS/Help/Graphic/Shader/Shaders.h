#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_
#include "class.Shader.h"

// TODO namespace Shader, public Base


/* tempalte ?
	
	
	template<typename BUFFER> 
	void AddBuffer() {
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		We set the fog start and fog end position in the constant buffer here by locking the buffer, setting the data, and then unlocking the buffer again. It is the second buffer in the vertex shader so we change the bufferNumber from 0 to 1.
		// Lock the fog constant buffer so it can be written to.
		result = deviceContext->Map(m_fogBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if(FAILED(result)){
			return false;
		}

		// Get a pointer to the data in the constant buffer.
		dataPtr2 = (FogBufferType*)mappedResource.pData;
		// Copy the fog information into the fog constant buffer.
		dataPtr2->fogStart = fogStart;
		dataPtr2->fogEnd = fogEnd;
		// Unlock the constant buffer.


		deviceContext->Unmap(m_fogBuffer, 0);
		// Set the position of the fog constant buffer in the vertex shader.
		bufferNumber = 1; // toto auto navysovat ?
	}
*/

class TextureShader : public Shader
{
public:
	void Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void Init(ID3D11Device* device, wstring name);
};


class LightShader : public Shader
{
public:
	LightShader();
	~LightShader();
	void Initialize(ID3D11Device* device, wstring vsFilename);
	void Render(
		ID3D11DeviceContext* deviceContext, 
		int indexCount,
		D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, 
		D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture,
		D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 diffuseColor
	);

protected:
	ID3D11Buffer* m_lightBuffer;

	struct LightBufferType {
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};
};

#endif