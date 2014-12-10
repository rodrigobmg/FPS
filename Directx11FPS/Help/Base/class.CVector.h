#ifndef H_CVector
#define H_CVector
#include <vector>
#include <algorithm>

// Dalsia implementacia:
// http://www.cplusplus.com/reference/algorithm/
// http://www.cppreference.com/wiki/algorithm/start

template <typename T> class CVector {
         std::vector<T> data;
		 virtual bool SortFunction(T &i, T &j) { return (i<j); } // pouzije sa pretazenie operatorov, alebo sa pretazi tato metoda
		 virtual void Print(T &a) { cout << a; }

	public:
		void Add(T const &d);
        void Remove();
        void Print();
		void Sort();
};

template <typename T> void CVector<T> ::Add(T const &d) {
     data.push_back(d);
}
template <typename T> void CVector<T>::Remove() {
      data.erase(data.begin( ) + 0,data.begin( ) + 1);
}
template <typename T> void CVector<T>::Print() {
     std::vector <int>::iterator It1;
     It1 = data.begin();
     for ( It1 = data.begin( ) ; It1 != data.end( ) ; It1++ ) Print(*It1);
}
template <typename T> void CVector<T>::Sort() {
	sort (data.begin(), data.end(), sortfunction);
}

#endif