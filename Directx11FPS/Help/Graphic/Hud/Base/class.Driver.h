#pragma once
#include <d3d11.h>
#include <d3dx11tex.h>
#include <vector>

#include "../../../../Global.h"
#include "../../../Base/class.Module.h"
#include "../../../Base/Structures/class.CVector.h"
#include "../../../App/class.Direct.h"
#include "../../../App/Base/struct.Screen.h"
#include "class.Item.h"

/*
	As already mentioned, use D3D10_PRIMITIVE_TOPOLOGY_LINELIST or D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP to draw line primitives. 
	These will be 3D lines. To draw 2D lines, give them all the same fixed Z coordinate. 
	Make sure that this Z coordinate is inside your view frustum
*/

/*
	Remember that different users will run your application in different resolutions. 
	One size font is not going to be clearly readable on all resolutions. 
	So you may want to make 3-4 different font sizes and use certain ones for certain resolutions to fix this problem. 
*/

/*
	Pozor treba vypinat ZBuffer a potom ho zase zapnut.
	Alpha blending riesi priesvitnost hudu.
	CreateRasterizerState tiez ovpivnuje hud renderovanie.
*/
namespace Hud
{

class Driver :
	public Module
{	
private:
	CVector<Item*> data;
	Screen m_screen;

public:
	// TaskHud Zobraz obrazok na 1 sec, potom pomaly z tmavni ( alpha-- ) a potom prec
	void Initialize(Screen& info) { 
		m_screen = info;
	}
	~Driver() { 
		data.RemoveAll();
	}
	Screen getScreenInfo() { 
		return m_screen; 
	}
	void Add(Item* d) {
		d->m_huddriver = this;
		data.Add(d);
	}
	bool Remove(Item* d) {
		if(data.Remove(d)) {
			d->m_huddriver = 0;
		}
	}

protected:
	void Frame(SFrame& s) {		
		gDirect->TurnZBuffer(false);	// Turn off the Z buffer to begin all 2D rendering.
		gDirect->TurnAlphaBlending(true); // Turn on the alpha blending before rendering the text.

		std::vector <Item*>::iterator It1;
		for ( It1 = data.begin( ) ; It1 != data.end( ) ; It1++ ) (*It1)->Render();

		gDirect->TurnAlphaBlending(false);	// Turn off alpha blending after rendering the text.
		gDirect->TurnZBuffer(true);	// Turn the Z buffer back on now that all 2D rendering has completed.
	}
};


}