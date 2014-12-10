#pragma once
#include "../../Base/System/class.Time.h"

class EasyFPS {
public:
	inline UINT GetFPS() {
		return m_fps;
	}

protected:
	void FPSCount() {
		m_lastfps++;
	
		// Rovanake ako preslo sekund ale pracuje stale
		double delta = Time::getInstance().GetAbsolute() - m_cas;
		if( delta > 1.0f )  {
			m_cas = Time::getInstance().GetAbsolute();
			m_fps = m_lastfps;
			m_lastfps = 0;
		}
	}
	EasyFPS() {
		m_cas = Time::getInstance().GetAbsolute();
		m_lastfps = 0;
		m_fps = 0;
	}

private:
	double m_cas; 
	UINT m_lastfps;
	UINT m_fps;
};