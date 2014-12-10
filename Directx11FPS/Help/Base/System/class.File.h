#ifndef H_File
#define H_File

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>
#include <string>
#include <exception>
#include "../String.h"

namespace File 
{
	// Exceptions
	class NotFound : public std::exception { 
	public:	
		NotFound(std::string txt) {
			txt = "File: "+txt+" not found";
			exception(txt.c_str());
		}
		NotFound(std::wstring txt) {
			txt.append(L"File: ").append(txt).append(L" not found");
			exception(ws2s(txt).c_str());
		}
	};

	class Processing : public std::exception {};

	void SplitPath(const std::string& inputPath, std::string* pathOnly, std::string* filenameOnly);
	bool Find(std::string *filename);
	bool DoesExist(const std::string &filename);
	std::wstring GetTheCurrentDirectory();

};

#endif