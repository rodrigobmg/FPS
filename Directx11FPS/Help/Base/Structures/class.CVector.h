#ifndef H_CVector
#define H_CVector
#include <vector>
#include <algorithm>
#include <iostream>

// Dalsia implementacia:
// http://www.cplusplus.com/reference/algorithm/
// http://www.cppreference.com/wiki/algorithm/start

template <typename T> 
class CVector 
	: public std::vector<T>
{
protected:
	virtual bool SortFunction(T &i, T &j) { return (i<j); } // pouzije sa pretazenie operatorov, alebo sa pretazi tato metoda
	virtual void Print(T &a) { std::cout << a; }

public:
	void Add(T const &d);
	bool Remove(T const &d);
    void RemoveAll();
    void Print();
	void Sort();
};

template <typename T> void CVector<T> ::Add(T const &d) {
    push_back(d);
}
template <typename T> void CVector<T>::RemoveAll() {
   erase(begin( ) + 0,begin( ) + 1);
}
template <typename T> bool CVector<T>::Remove(T const &d) {
   
}
template <typename T> void CVector<T>::Print() {
    std::vector <T>::iterator It1;
    for ( It1 = begin( ) ; It1 != end( ) ; It1++ ) Print(*It1);
}
template <typename T> void CVector<T>::Sort() {
	sort (begin(), end(), sortfunction);
}

#endif