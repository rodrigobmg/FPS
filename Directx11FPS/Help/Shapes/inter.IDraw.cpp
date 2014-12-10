#include "inter.IDraw.h"
using namespace Shapes;

Shaders::ColorObject::Shader* gIDraw_Defaultshader;

IDraw::Sustava::Sustava() {
			
}

void IDraw::Sustava::Render(vec &origin, const vec &size) 
{
	if(!Inicialized()) Inicialize();

	XMMATRIX matWorld = XMMatrixScaling(size.x, size.y, size.z); // TODO: neskor virtual vec getExtends() - zisti sirku|vysku|dlzku
	matWorld.r[3] = XMVectorSelect( matWorld.r[3], XMLoadFloat3(&origin), XMVectorSelectControl( 1, 1, 1, 0 ) );

	// copy to vertex buffer
	for( DWORD i=0; i < 4; ++i ) {
		XMVECTOR position = XMLoadFloat3( &Vertex()[i].position );
		position = XMVector3Transform(position, matWorld );
		XMStoreFloat3( &Vertex()[i].position, position);
	}
	SetVBuffer();
	FlushBuffers();
}

void IDraw::Sustava::Inicialize() {
	const UINT indices[] = {
		0, 1, 2, 1, 3
	};
	const Structures::VertexType vertices[] = {
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),  
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),  
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),  
		D3DXVECTOR3(0.0f, 0.0f, 1.0f)
	};

	CreateVStructure(4);
	CreateIStructure(indices, 5);
	CreateDVBuffer();
	CreateSIBuffer();
}


IDraw::IDraw() {
	setColor(vec4(1.f, 0.f, 0.f, 1.f));
}

// Vykresluj
void IDraw::Draw() { 
	if(!shape.Inicialized()) DrawInicialize();
	Drawer(); 
	gIDraw_Defaultshader->Features()->color = color;
	gIDraw_Defaultshader->Render();
	shape.FlushBuffers();
}


