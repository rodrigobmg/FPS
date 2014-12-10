#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundManager
{
public:
	SoundManager();
	virtual void Initialize(HWND);
	~SoundManager();

	// Vytvor bufferi pre pesnicky
	IDirectSoundBuffer8* CreateBuffer(DSBUFFERDESC *bufferDesc);
	IDirectSound3DBuffer8* Create3DBuffer(IDirectSoundBuffer8* secondaryBuffer);

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSound3DListener8* m_listener;
};

extern SoundManager* gSound;

#endif