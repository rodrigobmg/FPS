#include "DXUT.h"
#include "TextureLibray.h"

TextureLibrary *TextureLibrary::m_Singleton = NULL;

TextureLibrary::TextureLibrary()
{
}

void TextureLibrary::AddTexture(std::string name,ID3D10Texture2D*tex,ID3D10ShaderResourceView *srv)
{
	if(GetTexture(name)) return;

	tex->AddRef();
	srv->AddRef();

	// all is good?  Then add to our list
	m_Textures.insert(TextureMap::value_type(name,tex));
	m_ShaderResourceViews.insert(SRVMap::value_type(name,srv));
}

ID3D10Texture2D *TextureLibrary::GetTexture(std::string id)
{
	for(TextureMap::iterator it = m_Textures.begin();it!=m_Textures.end();it++)
	{
		if(strcmp(it->first.c_str(),id.c_str()) == 0)
		{
			return it->second;
		}
	}
	return NULL;
}

int TextureLibrary::GetTextureIndex(std::string id)
{
	int i=0;
	for(TextureMap::iterator it = m_Textures.begin();it!=m_Textures.end();i++,it++)
	{
		if(strcmp(it->first.c_str(),id.c_str()) == 0)
		{
			return i;
		}
	}
	return 0;
}

ID3D10Texture2D *TextureLibrary::GetTexture(int id)
{
	int i=0;
	for(TextureMap::iterator it = m_Textures.begin();i!= id || it!=m_Textures.end();i++,it++)
	{
		if(i==id)
		{
			return it->second;
		}
	}
	return NULL;
}

ID3D10ShaderResourceView *TextureLibrary::GetShaderResourceView(std::string id)
{
	for(SRVMap::iterator it = m_ShaderResourceViews.begin();it!=m_ShaderResourceViews.end();it++)
	{
		if(strcmp(it->first.c_str(),id.c_str()) == 0)
		{
			return it->second;
		}
	}
	return NULL;
}

void TextureLibrary::Release()
{
	for(TextureMap::iterator it = m_Textures.begin();it!= m_Textures.end();it++)
	{
		if(it->second)	SAFE_RELEASE(it->second);
	}
	m_Textures.clear();

	for(SRVMap::iterator it = m_ShaderResourceViews.begin();it!= m_ShaderResourceViews.end();it++)
	{
		if(it->second)	SAFE_RELEASE(it->second);
	}
	m_ShaderResourceViews.clear();
}
