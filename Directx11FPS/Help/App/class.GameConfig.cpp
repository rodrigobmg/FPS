#include <windows.h>
#include <string>
#include "class.GameConfig.h"
#include "../../Global.h"
#include "../Math/Math.h"
#include "../Base/String.h"
#include <iostream>
#include <fstream>
#include <exception>
using namespace std;

void GameConfig::Defaultne() {	
	m_debug = true;
	m_limitfps = false;
	m_name = "Player";
	m_file = "config.ini";

	m_pohyb_citlivost[0] = 8.0f;
	m_pohyb_citlivost[1] = 8.0f;
	m_pohyb_citlivost[2] = 8.0f;

	m_casovac = 0.000001f;
	m_oneskorenie = true;//true;

	m_kamera_citlivost[0] = (float) Math::UholNaRadiany(5.f);
	m_kamera_citlivost[1] = 3.0f;
}
GameConfig::GameConfig() {
	Defaultne();
}

bool GameConfig::LoadConfig() {
	string line;
	ifstream subor(m_file.c_str());
	// "Config subor nenajdeny, vytvaram novy."
	if(subor.is_open()) {
		vector<string> a;
		while(!subor.eof() ) {
			getline(subor, line);
			StringExplode(line, ":", &a);
			CMDParser(a);
		}
		subor.close();
		return true;
	} else {
		throw new exception("Subor nenajdeny / nemozem ho otvorit.");
		return false;
	}
	return false;
} 

bool GameConfig::SaveConfig() {
	ofstream subor(m_file.c_str());
	if (subor.is_open()) {
		char buffer [50];
		#define _CRT_SECURE_NO_WARNINGS
		sprintf(buffer, "name:%s\n", m_name);
		subor << buffer;
		sprintf(buffer, "debug:%s\n", (m_debug) ? "on" : "off");
		subor << buffer;
		subor.close();
		return true;
	}
	return false;
} 
