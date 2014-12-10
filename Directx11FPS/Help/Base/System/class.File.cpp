#include "class.File.h"
using namespace std;

/* 
	Try to find a texture file
	There is often pain finding the textures for .x files as sometimes the filename
	includes the path and other times it does not. It is a cause of a lot of issues. By
	saving your .x file as text you can look at it and see what the filenames are set to
	which can help spot problems.
*/

bool File::Find(string *filename) {
	if (!filename) return false;

	// Look for it first using the original filename and path
	if (DoesExist(*filename)) return true;

	// Next try with stripping the path from it
	string pathOnly;
	string filenameOnly;
	SplitPath(*filename,&pathOnly,&filenameOnly);

	if (DoesExist(filenameOnly)) {
		*filename=filenameOnly;
		return true;
	}
	// Could now try by looking in a specific data folder....
	return false;
}

/*
	Uses the basic C run time _access fucntion to determine if the file exists
*/

bool File::DoesExist(const string &filename) {
	return (_access(filename.c_str(), 0) != -1);
}

/*
	Splits a path and filename string
	pathOnly or filenameOnly can be null if they are not required
*/

void File::SplitPath(const string& inputPath, string* pathOnly, string* filenameOnly)
{
	string fullPath(inputPath);
	Replace(fullPath, "\\", "/");
	string::size_type lastSlashPos=fullPath.find_last_of('/');

	// check for there being no path element in the input
	if (lastSlashPos==string::npos) {
		*pathOnly = "";
		*filenameOnly = fullPath;
	} else { // do the split
		if (pathOnly) *pathOnly = fullPath.substr(0,lastSlashPos);
		if (filenameOnly) *filenameOnly = fullPath.substr(lastSlashPos+1,fullPath.size()-lastSlashPos-1);
	}
}

/*
	Retrieves the current directory
	Takes into account buffer size etc.
*/

wstring File::GetTheCurrentDirectory() {
	int bufferSize = GetCurrentDirectory(0,NULL);
	WCHAR *buffer;
	buffer = new WCHAR[bufferSize];
	GetCurrentDirectory(bufferSize, buffer);
	wstring directory(buffer);
	delete []buffer;
	return directory;
}
