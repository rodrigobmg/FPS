#pragma once
#include <string>
#include <map>

namespace std {

template <typename ID, typename DATA>
class CMap 
	: protected map<ID, DATA> 
{
protected:
	virtual DATA New(ID type) = 0;

public:
	DATA Request(ID type) {
		 std::map<ID, DATA>::iterator it;
		 it = find(type);
		 if(it == end()) {
			 DATA temp = New(type);
			 insert( pair<ID, DATA>(type,temp) );
			 return temp;
		 }
		 return it->second;
	}
	virtual ~CMap() {
		clear();
	}
};

}
