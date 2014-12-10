#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include "../../Base/System/File.h"
#include "inter.IBase.h"

namespace Structures 
{

struct RastertekVertexType {
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

class RastertekFile : 
	protected IBase<RastertekVertexType, UINT>
{
public:
	RastertekFile(char* filename) {
		m_filename = filename;
		Create();
	}
	void Render() {
		FlushBuffers();
	}

private:
	char* m_filename;

	void Create() {
		ifstream fin;
		char input;
		UINT i;

		// Open the model file.
		fin.open(m_filename);
	
		// If it could not open the file then exit.
		if(fin.fail()) {
			throw new File::NotFound(m_filename);
		}

		// Read up to the value of vertex count.
		fin.get(input);
		while(input != ':') {
			fin.get(input);
		}

		// Read in the vertex count.
		UINT count;
		fin >> count;
		CreateVStructure(count);
		CreateIStructure(count);

		// Read up to the beginning of the data.
		fin.get(input);
		while(input != ':') {
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		// Read in the vertex data.
		for(i=0; i< count; i++) {
			fin >> Vertex()[i].position.x >> Vertex()[i].position.y >> Vertex()[i].position.z;
			fin >> Vertex()[i].texture.x >> Vertex()[i].texture.y; // tu a tv
			fin >> Vertex()[i].normal.x >> Vertex()[i].normal.y >> Vertex()[i].normal.z;

			Index()[i] = i;
		}

		// Close the model file.
		fin.close();
		CreateSVBuffer();
		CreateSIBuffer();
	}
};



}