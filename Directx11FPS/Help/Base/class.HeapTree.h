#ifndef H_HeapTree
#define H_HeapTree
#include <assert.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <new>
#include <limits.h>

// http://www.codeproject.com/KB/architecture/heaptree.aspx
// http://algoritmicke-techniky.sk/zaciatocnik/1-4-Prioritna_fronta.html
// Classa musi mat bool Heapsort(TYPE *a) podla toho zoraduje


template <class TYPE> 
class HeapTree
{
public:
	HeapTree() { m_pData = NULL; m_nSize = 0; m_nMaxSize = 0; }
	HeapTree( const HeapTree <TYPE>& a ) { for( UINT i = 0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); }
	~HeapTree() { RemoveAll(); }
    HeapTree* operator=( const HeapTree <TYPE>& a );
    HRESULT Add( const TYPE& value );
    HRESULT Insert( UINT nIndex, const TYPE& value );
    HRESULT SetAt( UINT nIndex, const TYPE& value );
    UINT GetSize() const{ return m_nSize; }
	UINT Size() const{ return m_nSize; }
	bool IsEmpty() const{ return m_nSize == 0; }
	UINT Length() const{ return m_nSize; }
	TYPE* GetData() { return m_pData; }
    bool Contains( const TYPE& value ) { return ( -1 != IndexOf( value ) ); }
    UINT IndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? IndexOf( value, 0, m_nSize ) : -1; }
    UINT IndexOf( const TYPE& value, UINT iStart ) { return IndexOf( value, iStart, m_nSize - iStart ); }
    UINT IndexOf( const TYPE& value, UINT nIndex, UINT nNumElements );
    UINT LastIndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? LastIndexOf( value, m_nSize - 1, m_nSize ) : -1; }
    UINT LastIndexOf( const TYPE& value, UINT nIndex ) { return LastIndexOf( value, nIndex, nIndex + 1 ); }
    UINT LastIndexOf( const TYPE& value, UINT nIndex, UINT nNumElements );
    void RemoveAll() { SetSize(0); }
	inline TYPE GetTop() { return GetAt(0); }
	inline void RemoveTop() { Remove(0); }

private:
	// Tieto nechceme pouzivat
	const TYPE& operator[]( UINT nIndex ) const { return GetAt( nIndex );  }
    TYPE& operator[]( UINT nIndex ) { return GetAt( nIndex ); }
	HRESULT SetSize( UINT nNewMaxSize );
	TYPE& GetAt( UINT nIndex ) const { assert( nIndex >= 0 && nIndex < m_nSize ); return m_pData[nIndex]; } 
	HRESULT Remove( UINT nIndex );
	void Reset() {  m_nSize = 0; }

protected:
	TYPE* m_pData;      // the actual array of data
    UINT m_nSize;        // # of elements (upperBound - 1)
    UINT m_nMaxSize;     // max allocated
    HRESULT SetSizeInternal( UINT nNewMaxSize );  // This version doesn't call ctor or dtor.

	void Add(UINT iCur);
	void ReformatHeap(UINT root);
	inline UINT getLeft(UINT i) const { return 2*i + 1;}
	inline UINT getRight(UINT i) const { return i + 1; }
	inline UINT getParent(UINT i) const { return (i-1)/2; }
};


//--------------------------------------------------------------------------------------
// Implementation of HeapTree
//--------------------------------------------------------------------------------------

template<typename TYPE> 
HeapTree<TYPE>* HeapTree<TYPE>::operator=( const HeapTree <TYPE>& a ) { 
	if( this == &a ) return *this; 
	RemoveAll(); 
	for( UINT i = 0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); 
	return this; 
}

// This version doesn't call ctor or dtor.
template<typename TYPE> 
HRESULT HeapTree<TYPE>::SetSizeInternal( UINT nNewMaxSize )
{
    if( nNewMaxSize < 0 || ( nNewMaxSize > INT_MAX / sizeof( TYPE ) ) ) {
        assert( false );
        return E_INVALIDARG;
    }

    if( nNewMaxSize == 0 ) {
        // Shrink to 0 size & cleanup
        if( m_pData ) {
            free( m_pData );
            m_pData = NULL;
        }
        m_nMaxSize = 0;
        m_nSize = 0;
    }  else if( m_pData == NULL || nNewMaxSize > m_nMaxSize ) {
        // Grow array
        UINT nGrowBy = ( m_nMaxSize == 0 ) ? 16 : m_nMaxSize;
        // Limit nGrowBy to keep m_nMaxSize less than INT_MAX
        if( ( UINT )m_nMaxSize + ( UINT )nGrowBy > ( UINT )INT_MAX )
            nGrowBy = INT_MAX - m_nMaxSize;

        nNewMaxSize = __max( nNewMaxSize, m_nMaxSize + nGrowBy );
        // Verify that (nNewMaxSize * sizeof(TYPE)) is not greater than UINT_MAX or the realloc will overrun
        if( sizeof( TYPE ) > UINT_MAX / ( UINT )nNewMaxSize )
            return E_INVALIDARG;

        TYPE* pDataNew = ( TYPE* )realloc( m_pData, nNewMaxSize * sizeof( TYPE ) );
        if( pDataNew == NULL )
            return E_OUTOFMEMORY;

        m_pData = pDataNew;
        m_nMaxSize = nNewMaxSize;
    }
    return S_OK;
}
//--------------------------------------------------------------------------------------
template<typename TYPE> inline HRESULT HeapTree <TYPE>::SetSize( UINT nNewMaxSize )
{
    UINT nOldSize = m_nSize;
    if( nOldSize > nNewMaxSize ) {
        assert( m_pData );
        if( m_pData ) {
            // Removing elements. Call dtor.
            for( UINT i = nNewMaxSize; i < nOldSize; ++i )
                m_pData[i].~TYPE();
        }
    }
    // Adjust buffer.  Note that there's no need to check for error
    // since if it happens, nOldSize == nNewMaxSize will be true.)
    HRESULT hr = SetSizeInternal( nNewMaxSize );
    if( nOldSize < nNewMaxSize ) {
        assert( m_pData );
        if( m_pData ) {
            // Adding elements. Call ctor.
            for( UINT i = nOldSize; i < nNewMaxSize; ++i )
                ::new ( &m_pData[i] ) TYPE;
        }
    }
    return hr;
}
//--------------------------------------------------------------------------------------
template<typename TYPE> inline HRESULT HeapTree <TYPE>::Add( const TYPE& value )
{
    HRESULT hr;
    if( FAILED( hr = SetSizeInternal( m_nSize + 1 ) ) )
        return hr;

    // Construct the new element
    ::new ( &m_pData[m_nSize] ) TYPE;
    // Assign
    m_pData[m_nSize] = value;
	Add(m_nSize);
    ++m_nSize;
    return S_OK;
}
//--------------------------------------------------------------------------------------
template<typename TYPE> inline HRESULT HeapTree <TYPE>::Insert( UINT nIndex, const TYPE& value )
{
    HRESULT hr;
    // Validate index
    if( nIndex < 0 || nIndex > m_nSize ) {
        assert( false );
        return E_INVALIDARG;
    }

    // Prepare the buffer
    if( FAILED( hr = SetSizeInternal( m_nSize + 1 ) ) )
        return hr;

    // Shift the array
    MoveMemory( &m_pData[nIndex + 1], &m_pData[nIndex], sizeof( TYPE ) * ( m_nSize - nIndex ) );
    // Construct the new element
    ::new ( &m_pData[nIndex] ) TYPE;

    // Set the value and increase the size
    m_pData[nIndex] = value;
	Add(nIndex);
    ++m_nSize;

    return S_OK;
}


//--------------------------------------------------------------------------------------
template<typename TYPE> inline HRESULT HeapTree <TYPE>::SetAt( UINT nIndex, const TYPE& value )
{
    // Validate arguments
    if( nIndex < 0 ||  nIndex >= m_nSize ) {
        assert( false );
        return E_INVALIDARG;
    }
    m_pData[nIndex] = value;
	ReformatHeap(nIndex);
    return S_OK;
}
//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the first occurrence
// within the section of the data array that extends from iStart and contains the 
// specified number of elements. Returns -1 if value is not found within the given 
// section.
//--------------------------------------------------------------------------------------
template<typename TYPE> inline UINT HeapTree <TYPE>::IndexOf( const TYPE& value, UINT iStart, UINT nNumElements )
{
    // Validate arguments
    if( iStart < 0 || iStart >= m_nSize || nNumElements < 0 || iStart + nNumElements > m_nSize ) {
        assert( false );
        return -1;
    }
    // Search
    for( UINT i = iStart; i < ( iStart + nNumElements ); i++ ) {
        if( value == m_pData[i] )
            return i;
    }
    // Not found
    return -1;
}


//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the last occurrence
// within the section of the data array that contains the specified number of elements
// and ends at iEnd. Returns -1 if value is not found within the given section.
//--------------------------------------------------------------------------------------
template<typename TYPE> inline UINT HeapTree <TYPE>::LastIndexOf( const TYPE& value, UINT iEnd, UINT nNumElements )
{
    // Validate arguments
    if( iEnd < 0 || iEnd >= m_nSize || nNumElements < 0 || iEnd - nNumElements < 0 ) {
        assert( false );
        return -1;
    }
    // Search
    for( UINT i = iEnd; i > ( iEnd - nNumElements ); i-- ) {
        if( value == m_pData[i] ) return i;
    }
    // Not found
    return -1;
}
template<typename TYPE> inline HRESULT HeapTree <TYPE>::Remove( UINT nIndex )
{
    if( nIndex < 0 ||  nIndex >= m_nSize ) {
        assert( false );
        return E_INVALIDARG;
    }
    // Destruct the element to be removed
    m_pData[nIndex].~TYPE();
    // Compact the array and decrease the size
    MoveMemory( &m_pData[nIndex], &m_pData[nIndex + 1], sizeof( TYPE ) * ( m_nSize - ( nIndex + 1 ) ) );
    
	// move the last elem to the first place
	m_pData[0] = m_pData[-- m_nSize];
	ReformatHeap(0);
    return S_OK;
}

template <class TYPE>
inline void HeapTree<TYPE>::Add(UINT iCur) {
	UINT iParent = getParent(iCur); 
	while (iParent >= 0 && m_pData[iParent]->Heapsort(m_pData[iCur])) {
		TYPE t = m_pData[iCur];
		m_pData[iCur] = m_pData[iParent];
		m_pData[iParent] = t;
		iCur = iParent;
		iParent = getParent(iCur);
	}
}
template <class TYPE>
inline void HeapTree<TYPE>::ReformatHeap(UINT iRoot)
{
	UINT iChild = getLeft(iRoot); // left child index
	if (iChild < m_nSize)
	{
		UINT iRightChild = getRight(iChild); // right child index
		if (iRightChild < m_nSize) {
			if (m_pData[iChild]->Heapsort(m_pData[iRightChild])) {
				iChild = iRightChild;
			}
		}
		if (m_pData[iChild]->Heapsort(m_pData[iRoot])) {
			TYPE t = m_pData[iRoot];
			m_pData[iRoot] = m_pData[iChild];
			m_pData[iChild] = t;
			ReformatHeap(iChild);
		}
	}
}

#endif
