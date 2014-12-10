#include "class.Text.h"
#include "../../../App/class.Direct.h"
#include "../../../../Global.h"
#include "class.Driver.h"
using namespace Hud;

void Text::Initialize(UINT max)
{
	UINT i;
	max = max * 6;
	IBase::CreateVStructure( max );
	IBase::CreateIStructure( max );
	for(i=0; i< max; i++) {
		Index()[i] = i;
	}
	IBase::CreateDVBuffer();
	IBase::CreateSIBuffer();
}
void Text::Preset(UINT max)
{
	IBase::DeleteAll();
	Initialize(max);
}
UINT Text::getLength() {
	return IBase::GetVertexCount() / 6;
}
void Text::Set(char* text, vec2 position)
{
	m_text = text;
	m_position = position;	
	assert(m_font); // "Nastav font pre tento text"

	// Check for possible buffer overflow.
	UINT numLetters;
	numLetters = (int) strlen(text);
	if(numLetters > getLength() ) {
		Preset(numLetters);
	}

	//IBase::ZeroV();
	// Calculate the X and Y pixel position on the screen to start drawing to.
	assert(getDriver()); // "Hud item nemas priradeny do hud driveru."
	Screen* screen = & (getDriver()->getScreenInfo());

	float drawX, drawY;
	drawX = (float) ( (int) (screen->width / 2) * -1) + position.x;
	drawY = (float) (screen->height / 2) - position.y;
	// Use the font class to build the vertex array from the Text text and Text draw location.
	m_font->BuildVertexArray((void*) Vertex(), text, drawX, drawY);

	IBase::SetVBuffer();
}
void Text::Render()
{
	if(m_text == NULL || getLength() == 0) return;
	assert(m_fontshader); // "Nemas nastaveny font shader"
	m_fontshader->Features()->color = m_color;	
	m_fontshader->Render();
	FlushBuffers();
}