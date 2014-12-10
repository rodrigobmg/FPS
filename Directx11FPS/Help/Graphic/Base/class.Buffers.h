#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "../Inter/class.IGraphicAutoDevice.h"
#include "struct.MatrixList.h"

namespace Shaders 
{

class Buffers : public IGraphicAutoDevice
{
public:		
	Buffers();
	~Buffers();

	// Na zaciatku inicializuj
	void Create(DWORD buffers);
	void Add(std::size_t num);
	void Add(D3D11_BUFFER_DESC* desc);

	// Na koniec
	void Prepare(void* bufferdata, const bool ps);
	void PrepareMatrix(MatrixList* matrix);
	void Restart();

private:
	// Bufferi
	struct _Buffers {
		ID3D11Buffer* buffer;
		std::size_t size;
	};

	_Buffers* m_buffers;
	DWORD m_bufferscount;
	DWORD m_bufferactual;
	DWORD m_bufferactualrender[2]; // staticke ? teda pre vsetke HLSL ale do konca frame
	D3D11_MAPPED_SUBRESOURCE *m_mappedResource;
};

class BuffersAssistant {
protected:
	Buffers _m_buffer;

public:
	Buffers* Buffer() { return &_m_buffer; }
};

}