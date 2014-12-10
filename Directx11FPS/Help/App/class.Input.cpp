#include "../App/class.App.h"
#include "class.Input.h"
#include "../../Global.h"

Input::Input() {
	Reset();
	m_timer.Reset();
}
POINT Input::getPos() {
	return m_pos;
}
POINT Input::getRozdiel() {
	POINT v;
	v.x = m_oldpos.x - m_pos.x;
	v.y = m_oldpos.y - m_pos.y;
	return v;
}	
bool Input::getStredne() {
	// TODO IMPLEMENT
	return true;
}
void Input::Reset() {
	for(int i=0; i < 256; i++) {
		m_button[i] = false;
		m_press[i] = false;
		m_pressold[i] = false;
		m_time[i] = 0.0;
	}
}
void Input::WND_LBUTTONDOWN(WPARAM& key) {
	m_lave = true;
}
void Input::WND_LBUTTONUP(WPARAM& key) {
	m_lave = false;
}
void Input::WND_RBUTTONDOWN(WPARAM& key) {
	m_prave = true;
}
void Input::WND_RBUTTONUP(WPARAM& key) {
	m_prave = false;
}
void Input::WND_KEYUP(WPARAM& key) {
	m_button[key] = false;
}
void Input::WND_KEYDOWN(WPARAM& key) {
	m_button[key] = true;
	m_pressold[key] = false;
}
void Input::Frame() {
	// Myska 
	m_oldpos = m_pos;
	GetCursorPos(&m_pos);
	ScreenToClient(gGame->getWindow(), &m_pos);

	// Klavesnica
	ProcessPress();
}
void Input::ProcessPress() {
	for(int i=0; i < 256; i++) {
		if(m_button[i] && !m_press[i] && !m_pressold[i]) {			
			m_press[i] = true;
			m_pressold[i] = false;
			PressStart(i);
		} else {
			m_pressold[i] = true;
			m_press[i] = false;
		}
	}
}
void Input::PressStart(int i) {
	m_time[i] = m_timer.GetAbsoluteTime();
}
bool Input::getKeydown(WPARAM key) {
	return m_button[key];
}
bool Input::getPress(WPARAM key) {
	return m_press[key];
}
bool Input::getPress(WPARAM key, double time) {
	return ((m_timer.GetAbsoluteTime() - m_time[key]) < time);
}
double Input::getTimePress(WPARAM key) {
	return m_time[key];
}
