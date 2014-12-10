#pragma once
#include <d3d11.h>
#include <d3dx11tex.h>

#include "../../../Global.h"
#include "../../Base/Structures/class.DynArray.h"
#include "../Inter/class.IGraphicAutoDevice.h"
#include "../Inter/inter.IRender.h"

class ITexture : public IRender {
};

//++ Jedna textura
class Texture  : 
	public ITexture,
	public IGraphicAutoDevice
{
public:
	Texture() { 	
		m_texture = NULL;
	}
	Texture(LPCWSTR filename) { 	
		if(FAILED( D3DX11CreateShaderResourceViewFromFile(Direct()->GetDevice(), filename, NULL, NULL, &m_texture, NULL) ) ) {
			throw new std::exception("Texture not found.");
		} 
	}
	void Add(LPCWSTR filename) { 	
		assert(m_texture == NULL);
		if(FAILED( D3DX11CreateShaderResourceViewFromFile(Direct()->GetDevice(), filename, NULL, NULL, &m_texture, NULL) ) ) {
			throw new std::exception("Texture not found.");
		} 
	}
	~Texture() {
		SAFE_RELEASE(m_texture)
	}	
	void Render() {	
		// Set Base texture resource in the pixel Base.
		assert(m_texture != NULL);
		Direct()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);
	}

	ID3D11ShaderResourceView* GetTexture()  { return m_texture; }

private:
	ID3D11ShaderResourceView* m_texture;
};

//++ Mnozina textur
class Textures :
	public DynArray<ID3D11ShaderResourceView*>,
	public ITexture,
	public IGraphicAutoDevice
{
public:
	void Render() {
		// Set Base texture resource in the pixel Base.
		assert(GetSize() > 0);
		Direct()->GetDeviceContext()->PSSetShaderResources(0, GetSize(), GetData());
	}
	inline void Add(ID3D11Device* device, LPCWSTR filename) { 	
		ID3D11ShaderResourceView* temp;
		if(FAILED( D3DX11CreateShaderResourceViewFromFile(Direct()->GetDevice(), filename, NULL, NULL, &temp, NULL) ) ) {
			throw new std::exception("Texture not found.");
		} 
		DynArray<ID3D11ShaderResourceView*>::Add(temp);
	}
	inline ~Textures() {
		ReleaseAll();
	}
};

//++ Mnozina mnozin textur
class TexturesGroup : 
	public DynArray<Textures*>,
	public ITexture,
	public IGraphicAutoDevice
{
public:
	inline ~TexturesGroup() {
		RemoveAll();
	}	
	void Render() {
		// Set Shader texture resource in the pixel Base.
		assert(GetSize() > 0);
		ID3D11DeviceContext* deviceContext = Direct()->GetDeviceContext();
		Textures* a;

		for(DWORD i=0; i < GetSize(); i++) {
			a = GetAt(i);
			assert(a->GetSize() > 0);
			deviceContext->PSSetShaderResources(i, a->GetSize(), a->GetData());
		}
	}
};