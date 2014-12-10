#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include <assert.h>
#include "../../App/Base/Memory.h"
#include "../../../Global.h"
#include "../Inter/class.IGraphicAutoDevice.h"
#include "inter.IStructure.h"

namespace Structures
{

// ! This setup needs to match the VertexType stucture in the ModelClass and in the shader.

template<typename VERTEXTYPE, typename INDEXTYPE>
class IBase :
	public IGraphicAutoDevice,
	public IStructure
{
public:
	IBase();
	~IBase();

	inline UINT GetVertexCount() { return m_vertexCount; }
	inline UINT GetIndexCount() { return m_indexCount; }

	inline VERTEXTYPE* Vertex() { return m_vertices; }
	inline INDEXTYPE* Index() { return m_indices; }
	inline bool Rendered() { return m_rendered; }
	inline bool Inicialized() { return m_rendered; }

	inline void CreateVStructure(UINT count);
	inline void CreateIStructure(UINT count);
	inline void CreateIStructure(const INDEXTYPE *indices, UINT icount);

	inline void CreateDVBuffer();
	inline void CreateDIBuffer();
	inline void CreateSVBuffer();
	inline void CreateSIBuffer();

	inline void FlushBuffers();
	inline void SetVBuffer();
	inline void SetIBuffer();

	inline void DeleteAll();
	inline void DeleteVStructure();
	inline void DeleteIStructure();
	inline void DeleteVBuffer();
	inline void DeleteIBuffer();
protected:
	virtual D3D11_PRIMITIVE_TOPOLOGY getRenderMode() { return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }

private:
	VERTEXTYPE* m_vertices;
	INDEXTYPE *m_indices;
	UINT m_stride, m_offset;
	UINT m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	D3D11_MAPPED_SUBRESOURCE m_mappedResource;
	bool m_rendered;
};

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::DeleteAll() {
	DeleteVStructure();
	DeleteIStructure();
	DeleteVBuffer();
	DeleteIBuffer();
}
template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::DeleteVStructure() {
	SAFE_DELETE_ARRAY( m_vertices )
}
template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::DeleteIStructure() {
	SAFE_RELEASE( m_indexBuffer )
}
template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::DeleteVBuffer() {
	SAFE_RELEASE( m_vertexBuffer )
}
template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::DeleteIBuffer() {
	SAFE_DELETE_ARRAY( m_indices )
}
template<typename VERTEXTYPE, typename INDEXTYPE>
IBase<VERTEXTYPE, INDEXTYPE>::IBase() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_indexCount = 0;
	m_vertexCount = 0;

	m_vertices = 0;
	m_indices = 0;

	m_rendered = false;
	m_stride = sizeof(VERTEXTYPE); 
	m_offset = 0;
}
template<typename VERTEXTYPE, typename INDEXTYPE>
IBase<VERTEXTYPE, INDEXTYPE>::~IBase() {
	DeleteAll();
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::CreateVStructure(UINT count)
{
	// Set the number of indices to be the same as the vertex count.
	assert(count > 0);
	m_vertexCount = count;

	// Create the model using the vertex count that was read in.
	m_vertices = new VERTEXTYPE[count];
	if(!m_vertices) {
		throw new Memory::Exception();
	}
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::CreateIStructure(UINT count)
{
	// Set the number of indices to be the same as the vertex count.
	assert(count > 0);
	m_indexCount = count;

	// Create the model using the vertex count that was read in.
	m_indices = new INDEXTYPE[count];
	if(!m_indices) {
		throw new Memory::Exception();
	}
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::CreateIStructure(const INDEXTYPE* indices, UINT icount)
{
	// Set the number of indices to be the same as the vertex count.
	CreateIStructure(icount);
	memcpy(m_indices, indices, sizeof(INDEXTYPE)*icount);
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::FlushBuffers() {
	// Flush
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
    assert(m_vertexBuffer || m_indexBuffer);
	m_rendered = true;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	if(m_vertexBuffer) Direct()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	if(m_indexBuffer) Direct()->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	Direct()->GetDeviceContext()->IASetPrimitiveTopology(getRenderMode());

	// Render the triangle.
	assert(m_indexCount > 0 );
	Direct()->GetDeviceContext()->DrawIndexed(m_indexCount, 0, 0);
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::CreateSVBuffer()
{
	assert(m_vertices);
	D3D11_BUFFER_DESC vertexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData;

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
    vertexBufferDesc.ByteWidth = m_stride * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0; 
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource StaticStructure a pointer to the vertex data.
    vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	if(FAILED( Direct()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer) )) {
		throw new std::exception();
	}
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::CreateSIBuffer()
{
	assert(m_indices);
	D3D11_BUFFER_DESC indexBufferDesc;
    D3D11_SUBRESOURCE_DATA indexData;

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(INDEXTYPE) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource StaticStructure a pointer to the index data.
    indexData.pSysMem = m_indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	if(FAILED( Direct()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer) )) {
		throw new std::exception();
	}
}


template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::CreateDVBuffer()
{
	assert(!m_vertexBuffer);
	D3D11_BUFFER_DESC vertexBufferDesc;

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = m_stride * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // 0
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Now create the vertex buffer.
	if(FAILED( Direct()->GetDevice()->CreateBuffer(&vertexBufferDesc, NULL, &m_vertexBuffer) )) {
		throw new std::exception();
	}
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::CreateDIBuffer()
{
	assert(!m_indexBuffer);
	D3D11_BUFFER_DESC indexBufferDesc;

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    indexBufferDesc.ByteWidth = sizeof(INDEXTYPE) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Create the index buffer.
	if(FAILED( Direct()->GetDevice()->CreateBuffer(&indexBufferDesc, NULL, &m_indexBuffer) )) {
		throw new std::exception();
	}
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::SetVBuffer() 
{
	assert(m_vertices);

	// Lock the constant Buffers so it can be written to.
	if(FAILED( Direct()->GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mappedResource) ) ) {
		throw new std::exception("Map error.");
	}

	// Copy the variables into the constant Buffers.
	memcpy(m_mappedResource.pData, m_vertices, m_stride * m_vertexCount);

	// Unlock the constant Buffers.
	Direct()->GetDeviceContext()->Unmap(m_vertexBuffer, 0);
}

template<typename VERTEXTYPE, typename INDEXTYPE>
inline void IBase<VERTEXTYPE, INDEXTYPE>::SetIBuffer() 
{
	assert(m_indices);

	// Lock the constant Buffers so it can be written to.
	if(FAILED(  Direct()->GetDeviceContext()->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mappedResource) ) ) {
		throw new std::exception("Map error.");
	}

	// Copy the variables into the constant Buffers.
	memcpy(m_mappedResource.pData, m_indices, sizeof(INDEXTYPE) * m_indexCount);

	// Unlock the constant Buffers.
	 Direct()->GetDeviceContext()->Unmap(m_indexBuffer, 0);
}



}
