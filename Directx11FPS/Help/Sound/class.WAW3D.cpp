#include "class.WAW3D.h"
#include "../../Global.h"
#include <stdio.h>
#include <assert.h>
#include "class.SoundManager.h"

WAW3D::WAW3D() {
	m_Buffer = 0;
	m_3DBuffer = 0;
}
void WAW3D::Play() {
	m_Buffer->Play(0, 0, 0);
}
void WAW3D::RePlay() {
	SetPlayPosition(0);
}
void WAW3D::SetPosition(D3DVECTOR *x) {
	m_3DBuffer->SetPosition(x->x, x->y, x->z, DS3D_IMMEDIATE);		
}
void WAW3D::GetPosition(D3DVECTOR *x) {
	m_3DBuffer->GetPosition(x);
}
void WAW3D::Stop() {
	if(m_pause) return;
	m_pause = true;
	m_Buffer->Stop();
}
void WAW3D::Continue() {
	if(!m_pause) return;
	m_pause = false;
	m_Buffer->Restore();
}
void WAW3D::SetVolume(float x) {
	assert( x >= 0.0f && x <= 1.0 );
	m_Buffer->SetVolume((long) (-10000.f * x));
}
float WAW3D::GetVolume() {
	LONG x;
	m_Buffer->GetVolume(&x);
	return ( x / -10000.f );
}
void WAW3D::Init(char * name) {
	Load(name);
	m_name = name;
	D3DVECTOR init = {0.f, 0.f, 0.f};
	SetPosition(&init);
	SetPlayPosition(0);
	m_pause = false;
}
void WAW3D::SetPlayPosition(DWORD x) {
	m_Buffer->SetCurrentPosition(x);
}
const char* WAW3D::GetName() {
	return m_name;
}
DWORD WAW3D::GetPlayPosition() {
	DWORD a, b;
	m_Buffer->GetCurrentPosition(&a, &b);
	return a;
}
WAW3D::~WAW3D() {
	SAFE_RELEASE(m_Buffer)
	SAFE_RELEASE(m_3DBuffer)
}

void WAW3D::CheckWaveType(HeaderType *waveFileHeader) {
	// Check that the chunk ID is the RIFF format.
	if((waveFileHeader->chunkId[0] != 'R') || (waveFileHeader->chunkId[1] != 'I') || 
	   (waveFileHeader->chunkId[2] != 'F') || (waveFileHeader->chunkId[3] != 'F')) {
		throw new File::Processing();
	}

	// Check that the file format is the WAVE format.
	if((waveFileHeader->format[0] != 'W') || (waveFileHeader->format[1] != 'A') ||
	   (waveFileHeader->format[2] != 'V') || (waveFileHeader->format[3] != 'E')) {
		throw new File::Processing();
	}

	// Check that the sub chunk ID is the fmt format.
	if((waveFileHeader->subChunkId[0] != 'f') || (waveFileHeader->subChunkId[1] != 'm') ||
	   (waveFileHeader->subChunkId[2] != 't') || (waveFileHeader->subChunkId[3] != ' ')) {
		throw new File::Processing();
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if(waveFileHeader->audioFormat != WAVE_FORMAT_PCM) throw new File::Processing();

	// Check that the wave file was recorded in mono format.
	if(waveFileHeader->numChannels != 1)  throw new File::Processing();

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if(waveFileHeader->sampleRate != 44100) throw new File::Processing();

	// Ensure that the wave file was recorded in 16 bit format.
	if(waveFileHeader->bitsPerSample != 16) throw new File::Processing();

	// Check for the data chunk header.
	if((waveFileHeader->dataChunkId[0] != 'd') || (waveFileHeader->dataChunkId[1] != 'a') ||
	   (waveFileHeader->dataChunkId[2] != 't') || (waveFileHeader->dataChunkId[3] != 'a')) {
		throw new File::Processing();
	}
}

void WAW3D::Load(char *name) {
	// Open the wave file in binary.
	FILE* filePtr;
	if(fopen_s(&filePtr, name, "rb") != 0) {
		throw new File::NotFound(name);
	}

	// Read in the wave file header.
	HeaderType waveFileHeader;
	UINT count;
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1) throw new File::Processing();
	
	try { // :D
		CheckWaveType(&waveFileHeader);
		CreateBuffer(&waveFileHeader, filePtr);
		m_3DBuffer = gSound->Create3DBuffer(m_Buffer);
	} catch ( std::exception e) {
		fclose(filePtr);
		throw e;
	}
	
	// Close the file once done reading.
	fclose(filePtr);
}

void WAW3D::CreateBuffer(HeaderType *waveFileHeader, FILE* filePtr) 
{
	HRESULT result;

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader->dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
	m_Buffer = gSound->CreateBuffer(&bufferDesc);

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(HeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	unsigned char* waveData;
	waveData = new unsigned char[waveFileHeader->dataSize];
	if(!waveData) {
		throw new File::Processing();
	}

	// Read in the wave file data into the newly created buffer.
	UINT count = fread(waveData, 1, waveFileHeader->dataSize, filePtr);
	if(count != waveFileHeader->dataSize) throw new File::Processing();

	// Lock the secondary buffer to write wave data into it.
	unsigned char* bufferPtr;
	unsigned long bufferSize;
	result = m_Buffer->Lock(0, waveFileHeader->dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result)) {
		throw new File::Processing();
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader->dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = m_Buffer->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if(FAILED(result)) {
		throw new File::Processing();
	}
	
	// Release the wave data since it was copied into the secondary buffer.
	delete [] waveData;
	waveData = 0;
}