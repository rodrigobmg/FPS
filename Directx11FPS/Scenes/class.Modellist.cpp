///////////////////////////////////////////////////////////////////////////////
// Filename: modellistclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "class.ModelList.h"

bool ModelList::Initialize(int numModels)
{
	// Store the number of models.
	m_modelCount = numModels;
	float red, green, blue;
	vec position;
	int i;

	// Create a list array of the model information.
	m_ModelInfoList = new Sphere[m_modelCount];

	// Seed the random generator with the current time.
	srand((unsigned int)time(NULL));

	// Go through all the models and randomly generate the model color and position.
	for(i=0; i<m_modelCount; i++)
	{
		// Generate a random color for the model.
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;

		// Generate a random position in front of the viewer for the mode.
		m_ModelInfoList[i].setColor(vec4(red, green, blue, 1.0f));
		position.Random(-10.f, 10.f);
		//position.x = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		//position.y = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		//position.z = ((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;
		m_ModelInfoList[i].setOrigin(position);
		float r = ((600.f-1.0f)*((float)rand()/RAND_MAX))+1.0f;
		m_ModelInfoList[i].setRadius( r );
	}

	return true;
}


void ModelList::Shutdown()
{
	// Release the model information list.
	if(m_ModelInfoList)
	{
		delete [] m_ModelInfoList;
		m_ModelInfoList = 0;
	}

	return;
}


int ModelList::GetModelCount()
{
	return m_modelCount;
}


Sphere* ModelList::GetData(int index) {
	return &(m_ModelInfoList[index]);
}