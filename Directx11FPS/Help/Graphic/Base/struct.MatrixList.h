#pragma once
#include <d3d11.h>
#include <d3dx10math.h>


namespace Shaders 
{

struct MatrixList {
	MatrixList() {}
	MatrixList(	D3DXMATRIX w, D3DXMATRIX v, D3DXMATRIX p) {
		world = w;
		view = v;
		projection = p;
	}

	//union {
		//struct {
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;
		//};

		//D3DXMATRIX m[3];
	//};
};


}