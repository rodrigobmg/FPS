#pragma once
#include "../Base/System/class.File.h"
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

class WAW3D
{
public:
	WAW3D();
	virtual void Init(char * name);
	~WAW3D();

	// Ovladaj zvucku
	void Continue();
	void Stop();
	void Play();
	void RePlay();

	// nastav nejake vlastnosti
	void SetPosition(D3DVECTOR *x);
	void GetPosition(D3DVECTOR *x);
	void SetVolume(float x);
	float GetVolume();
	void SetPlayPosition(DWORD x);
	DWORD GetPlayPosition();
	const char* GetName();

private:
	bool m_pause;
	const char *m_name;

	struct HeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
	
	void Load(char * name);
	void CheckWaveType(HeaderType *waveFileHeader); 
	void CreateBuffer(HeaderType *waveFileHeader, FILE* filePtr);

	IDirectSoundBuffer8* m_Buffer;
	IDirectSound3DBuffer8* m_3DBuffer;
};
