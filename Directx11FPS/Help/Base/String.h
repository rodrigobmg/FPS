#ifndef H_String
#define H_String

// Pre zmenu LPWSTR, vseobecne pouzitie retazcov treba:
// projekt -> configurtion properties -> general -> character set -> use multi-byte character set
// http://www.daniweb.com/software-development/cpp/threads/87362

#include <string>
#include <sstream>
#include <vector>
using namespace std;

inline void StringExplode(string str, string separator, vector<string>* results)
{
    int found;
    found = str.find_first_of(separator);
    while(found != string::npos){
        if(found > 0){
            results->push_back(str.substr(0,found));
        }
        str = str.substr(found+1);
        found = str.find_first_of(separator);
    }
    if(str.length() > 0){
        results->push_back(str);
    }
}

// Pomocny template na konverzaciu INT, FLOTA do string
template <class T>
inline std::string ToString(const T & t, const int precision = 2)
{
  std::ostringstream oss;
  if(precision > 0) oss.precision( precision);
  oss.setf(std::ios_base::fixed);
  oss << t; 
  return oss.str();
}

#define CP_ACP 0
inline std::wstring s2ws(const std::string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
inline std::string ws2s(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
	std::string r(buf);
	delete[] buf;
	return r;
}

// his goes into an infinite loop if newstr contains oldstr: string test = "sand"; replace(test, "and", "panda");
inline void Replace(std::string & target, std::string oldstr, std::string newstr) {
    unsigned int x;
    while(x = target.find(oldstr), x != std::string::npos) {
        target.erase(x, oldstr.length());
        target.insert(x, newstr);
    }
} 

#endif
