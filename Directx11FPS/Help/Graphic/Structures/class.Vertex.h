#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "inter.IBase.h"

namespace Structures 
{
	struct VertexType {
		D3DXVECTOR3 position;
	};

	class DrawObject :
		public IBase<VertexType, UINT> {
	protected:
		virtual D3D11_PRIMITIVE_TOPOLOGY getRenderMode()  {
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		}
	};

}