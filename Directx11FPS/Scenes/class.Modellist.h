///////////////////////////////////////////////////////////////////////////////
// Filename: modellistclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_
#include "../Help/Shapes/Shapes.h"
using namespace Shapes;

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include <stdlib.h>
#include <time.h>


///////////////////////////////////////////////////////////////////////////////
// Class name: ModelList
///////////////////////////////////////////////////////////////////////////////
class ModelList
{
public:
	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	Sphere* GetData(int);

private:
	int m_modelCount;
	Sphere* m_ModelInfoList;
};

#endif