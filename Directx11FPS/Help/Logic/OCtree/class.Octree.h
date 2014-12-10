#pragma once
#include "class.BaseOCTree.h"
#include "../../../Entity/class.Entities.h"


// Porozdeluj objekty a terrain do OCtree

class OCTree : public BaseOCTree<OCTree>
{
public:
	// Implementacia Terrainu
	//class Terrain {
	/*
		Render the buffers in this node as normal if they can be seen using the terrain shader.
		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType); 
		offset = 0;
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		device->IASetVertexBuffers(0, 1, &node->vertexBuffer, &stride, &offset);
		// Set the index buffer to active in the input assembler so it can be rendered.
		device->IASetIndexBuffer(node->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Determine the number of indices in this node.
		indexCount = node->triangleCount * 3;
		// Call the terrain shader to render the polygons in this node.
		shader->RenderShader(device, indexCount);
	*/
		UINT m_triangles;
	//};

	// Implementacia Entities
	//class Entities {



	//};

	void ResetEntities() {
		// Entities* movedentities;
	}

	void Frame() { 
		// Check to see if the node can be viewed, height doesn't matter in a quad tree.
		//m_isvisible = frustum->CheckCube(node->positionX, 0.0f, node->positionZ, (node->width / 2.0f));
		ResetEntities(); 

		if(m_isvisible) Render(); 
	}
	void Render() {
		//Terrain::Render(i);
		//Entities::Render(i);
		BaseOCTree<OCTree>::Render();
	}

	// Dalsia implementacia
	bool m_isvisible;
	Entities *m_entities; // len tie presne v tej oblasti, ak su medzi tak su v parente
};