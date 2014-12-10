#pragma once
#include "../../Help/Graphic/Structures/class.RastertekFile.h"
#include "../../Help/Graphic/Shaders/Shaders.h"
#include "../../Help/Graphic/Base/class.Texture.h"
#include "class.Base.h"

namespace Entity {

// Shader + model zakomponovat do Box
class Sphere : public Entity::Base
{
private:
	Shaders::Light::Shader* m_LightShader;
	Structures::RastertekFile* m_Model;

public:
	Sphere() {
		static bool once = false;
		if(!once) {
			// Create the model object.
			m_Model = new Structures::RastertekFile("../Media/Structures/sphere.txt");
	
			// Create the light shader object.
			m_LightShader = new Shaders::Light::Shader();
			m_LightShader->Initialize();
			m_LightShader->Features()->texture.Add(L"../Media/Textures/seafloor.dds");
			m_LightShader->Features()->light.diffuseColor = vec4(1.0, 0.0, 0.0, 1.0);
			m_LightShader->Features()->light.lightDirection = vec(0.0f, 0.0f, 1.0f);
		}

		Shapes::Sphere shape = new Shapes::Sphere(vec(0.f), 1000.f);
		shape->setColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Entity::IPhysics::setShape(shape);
	}
	virtual void Draw() {
		// Move the model to the location it should be rendered at.
		mat.world = *gDirect->GetWorldMatrix();
		D3DXMATRIX a, b;
		D3DXMatrixTranslation(&a, sphere->getOrigin().x, sphere->getOrigin().y, sphere->getOrigin().z); // !!!
		float r = sphere->getRadius();
		D3DXMatrixScaling(&b, r, r, r);
		// ze vraj tieto funkcie robia kopie a a to sa riesi nasobenim
		D3DXMatrixMultiply(&mat.world, &a, &b);
				
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_LightShader->Features()->matrix = mat;
		m_LightShader->Features()->light.diffuseColor = sphere->getColor();

		// Render the model using the light shader.
		m_LightShader->Render();
		m_Model->Render();
	}
	virtual void setShape(Shapes::Base *s) { 

	}

};


}