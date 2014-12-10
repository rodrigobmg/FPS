#include "class.SoundManager.h"
#include "../../Global.h"
#include <exception>
using namespace std;

SoundManager::SoundManager() {
	m_DirectSound = 0;
	m_primaryBuffer = 0;
	m_listener = 0;
}

SoundManager::~SoundManager() {
	SAFE_RELEASE(m_listener)
	SAFE_RELEASE(m_primaryBuffer)
	SAFE_RELEASE(m_DirectSound)
}

void SoundManager::Initialize(HWND hwnd)
{
	// Initialize the direct sound interface pointer for the default sound device.
	HRESULT result;
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if(FAILED(result)) {
		throw new exception("Direct sound cant initialize");
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if(FAILED(result)) {
		throw new exception("Direct sound cant initialize");
	}

	// Setup the primary buffer description.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if(FAILED(result)) {
		throw new exception("Direct sound cant initialize");
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result)){
		throw new exception("Direct sound cant initialize");
	}

	// Obtain a listener interface.
	result = m_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_listener);
	if(FAILED(result)){
		throw new exception("Direct sound cant initialize");
	}

	// Set the initial position of the listener to be in the middle of the scene.
	m_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);
}

IDirectSoundBuffer8* SoundManager::CreateBuffer(DSBUFFERDESC *bufferDesc) {
	// Create a temporary sound buffer with the specific buffer settings.
	HRESULT result;
	IDirectSoundBuffer8* Buffer;
	IDirectSoundBuffer* tempBuffer;
	result = m_DirectSound->CreateSoundBuffer(bufferDesc, &tempBuffer, NULL);
	if(FAILED(result)) { 
		throw new exception("Direct sound cant create buffer");
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&Buffer);
	if(FAILED(result)) {
		throw new exception("Direct sound cant create buffer");
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;
	return Buffer;
}

IDirectSound3DBuffer8* SoundManager::Create3DBuffer(IDirectSoundBuffer8* secondaryBuffer) {
	// Get the 3D interface to the secondary sound buffer.
	IDirectSound3DBuffer8* secondary3DBuffer;
	secondaryBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&secondary3DBuffer);
	if(FAILED( secondaryBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&secondary3DBuffer) )) {
		throw new exception("Direct sound cant create buffer");
	}

	return secondary3DBuffer;
}