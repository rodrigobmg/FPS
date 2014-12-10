#pragma once
#include <exception>

/*
	Nejaka zakaldna sprava pamete
*/

namespace Memory 
{

class Exception : public std::exception {
public:
	Exception() {
		// Skus najst pamet , ries problem
		
		
		
		// A ukonci
	}

	// Nastav spravu 
	virtual const char* what() const throw() {
		return "Not enought memory.";
	}
};


}