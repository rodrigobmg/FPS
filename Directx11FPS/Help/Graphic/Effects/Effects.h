#pragma once
// http://www.asawicki.info/news_1371_effects_in_directx_11.html

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3dx11async.h>
#include "d3dx11effect.h"
#include "d3dxGlobal.h"
#include <assert.h>

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

//#pragma comment(lib, "Effects11.lib")

inline void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	fout << shaderFilename;
	fout << endl;
	for(i=0; i<bufferSize; i++) {
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	SAFE_RELEASE( errorMessage )

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	throw new exception("Error compiling shader.  Check shader-error.txt for message.");
}

void CreateEffect(ID3DX11Effect **g_Effect, ID3D11Device *dev, LPCWSTR file, LPCSTR type, UINT uFlags = 0) { // "fx_5_0"
	// Compile effect from HLSL file into binary Blob in memory
	ID3D10Blob *effectBlob = 0, *errorsBlob = 0; 
	HRESULT hr = D3DX11CompileFromFile(file, 0, 0, 0, type, D3D10_SHADER_ENABLE_STRICTNESS|D3D10_SHADER_DEBUG, 0, 0, &effectBlob, &errorsBlob, 0);
	//assert(SUCCEEDED(hr) && effectBlob);

	if(errorsBlob && effectBlob == 0) {
		OutputShaderErrorMessage(errorsBlob, file);
	} else {
		// If there was nothing in the error message then it simply could not find the shader file itself.
		//throw new exception("Missing Shader File");
	}


	if (errorsBlob) errorsBlob->Release();

	// Create D3DX11 effect from compiled binary memory block
	hr = D3DX11CreateEffectFromMemory(effectBlob->GetBufferPointer(), effectBlob->GetBufferSize(), uFlags, dev, g_Effect);
	assert(SUCCEEDED(hr));
	effectBlob->Release();
}



/*
ID3DX11EffectTechnique *g_EffectTechnique; // No need to be Release()-d.
g_EffectTechnique = g_Effect->GetTechniqueByIndex(0);
assert(g_EffectTechnique && g_EffectTechnique->IsValid());

ID3DX11EffectPass *g_EffectPass; // No need to be Release()-d.
g_EffectPass = g_EffectTechnique->GetPassByIndex(0);
assert(g_EffectPass && g_EffectPass->IsValid());
*/