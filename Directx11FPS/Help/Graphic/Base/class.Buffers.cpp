#include "class.Buffers.h"
#include "../../../Global.h"

using namespace Shaders;
using namespace std;

Buffers::Buffers() {
	m_buffers = 0;
	m_bufferscount = 0;
	m_bufferactual = 0;
	m_bufferactualrender[0] = 0;
	m_bufferactualrender[1] = 0;

	m_mappedResource = 0;
}

Buffers::~Buffers() {
	for(DWORD i=0; i < m_bufferscount; i++) {
		SAFE_RELEASE( m_buffers[i].buffer )
	}
	SAFE_DELETE_ARRAY(m_buffers)
	SAFE_DELETE(m_mappedResource)
}

void Buffers::Create(DWORD buffers) {
	// Priprav bufferi
	assert(!m_buffers);
	m_bufferscount = buffers;
	m_buffers = new _Buffers[buffers];
	assert(m_buffers);

	// Priprav pomocne premenne
	m_mappedResource = new D3D11_MAPPED_SUBRESOURCE;
}

void Buffers::PrepareMatrix(MatrixList* matrix) {
	// Transpose the matrices to prepare them for the Shader.
	assert(matrix);
	/*D3DXMatrixTranspose(&matrix->world, &matrix->world);
	D3DXMatrixTranspose(&matrix->view, &matrix->view);
	D3DXMatrixTranspose(&matrix->projection, &matrix->projection);
	*/

	MatrixList buffer;
	D3DXMatrixTranspose(&buffer.world, &matrix->world);
	D3DXMatrixTranspose(&buffer.view, &matrix->view);
	D3DXMatrixTranspose(&buffer.projection, &matrix->projection);



	Prepare(&buffer, false);
}

void Buffers::Prepare(void* bufferdata, const bool ps) {
	/*
		However, because Direct3D may be working with a Buffers in the background, 
		it never gives you direct CPU access to it.  In order to access it, the Buffers must be mapped.  
		This means that Direct3D allows anything being done to the Buffers to finish, 
		then blocks the GPU from working with the Buffers until it is unmapped.
	*/
	assert(bufferdata);
	ID3D11DeviceContext* deviceContext = Direct()->GetDeviceContext();
	DWORD id = m_bufferactualrender[0]+m_bufferactualrender[1]; 
	assert(id < m_bufferscount); // Nezabudol si Restart ?

	// Lock the constant Buffers so it can be written to.
	if(FAILED( deviceContext->Map(m_buffers[id].buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, m_mappedResource) ) ) {
		throw new exception("Map error.");
	}

	// Copy the variables into the constant Buffers.
	memcpy(m_mappedResource->pData, bufferdata, m_buffers[id].size);

	// Unlock the constant Buffers.
	deviceContext->Unmap(m_buffers[id].buffer, 0);

	// Finally set the constant Buffers in the pixel shader with the updated values.
	// Last, we unmap the Buffers.  This reallows the GPU access to the Buffers, and reblocks the CPU.  
	if(ps) {
		deviceContext->PSSetConstantBuffers(m_bufferactualrender[1], 1, &m_buffers[id].buffer);
	} else {
		deviceContext->VSSetConstantBuffers(m_bufferactualrender[0], 1, &m_buffers[id].buffer);
	}
	m_bufferactualrender[(int) ps]++;
}

void Buffers::Add(size_t num)
{
	// Setup the description of the light dynamic constant Buffers that is in the pixel shader.
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = num;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	Add(&desc);
}

void Buffers::Add(D3D11_BUFFER_DESC* desc)
{
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	if(desc->BindFlags == D3D11_BIND_CONSTANT_BUFFER && ((desc->ByteWidth % 16) != 0)) {
		throw new exception("ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER.");
	}

	// Create the constant Buffers pointer so we can access the vertex shader constant Buffers from within this class.
	assert(m_bufferscount != 0);
	assert(m_bufferactual < m_bufferscount);
	m_buffers[m_bufferactual].size = desc->ByteWidth;
	if(FAILED( Direct()->GetDevice()->CreateBuffer(desc, NULL, &(m_buffers[m_bufferactual].buffer)) )){
		throw new exception("CreateBuffer error.");
	}
	m_bufferactual++;
}
void Buffers::Restart() {
	assert(m_bufferactual == m_bufferscount); // Neboli vsetke bufferi inicializovane TODO: Neskor spravit DynArray<_buffer>
	assert(m_bufferactualrender[0]+m_bufferactualrender[1] == m_bufferscount); // Neboli vsetke bufferi pipravene

	m_bufferactualrender[0] = 0;
	m_bufferactualrender[1] = 0;

	SAFE_DELETE(m_mappedResource)
	m_mappedResource = new D3D11_MAPPED_SUBRESOURCE;
}
/*virtual void Flush() { // z IShader
	for(DWORD i=0; i < GetSize(); i++) {
		Prepare(i);
	}
	Restart();
}*/