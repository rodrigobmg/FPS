#ifndef H_VecConvectors
#define H_VecConvectors
#pragma once

#include "class.vec.h"
#define XM_NO_ALIGNMENT
#define _XM_NO_INTRINSICS_
#include <xnamath.h>

inline XMFLOAT3 vec2XMFLOAT3(const vec *a) {
	XMFLOAT3 r; 
	r.x = a->x;
	r.y = a->y;
	r.z = a->z;
	return r;
}
inline XMVECTOR vec42XMVECTOR(const vec4* pDestination) {
    XMVECTOR V;
    XMASSERT(pDestination);

    ((UINT *)(&V.vector4_f32[0]))[0] = ((const UINT *)(&pDestination->x))[0];
    ((UINT *)(&V.vector4_f32[1]))[0] = ((const UINT *)(&pDestination->y))[0];
    ((UINT *)(&V.vector4_f32[2]))[0] = ((const UINT *)(&pDestination->z))[0];
    ((UINT *)(&V.vector4_f32[3]))[0] = ((const UINT *)(&pDestination->w))[0];
    return V;
}
inline XMVECTOR vec2XMVECTOR(const vec* pDestination) {
    XMVECTOR V;
    XMASSERT(pDestination);

    ((UINT *)(&V.vector4_f32[0]))[0] = ((const UINT *)(&pDestination->x))[0];
    ((UINT *)(&V.vector4_f32[1]))[0] = ((const UINT *)(&pDestination->y))[0];
    ((UINT *)(&V.vector4_f32[2]))[0] = ((const UINT *)(&pDestination->z))[0];
    return V;
}




inline void XMStoreFloat4( vec4* pDestination, FXMVECTOR V) {    
    XMASSERT(pDestination);
	pDestination->x = V.vector4_f32[0];
    pDestination->y = V.vector4_f32[1];
    pDestination->z = V.vector4_f32[2];
    pDestination->w = V.vector4_f32[3];
}

inline void XMStoreFloat3( vec* pDestination, FXMVECTOR V) {    
    XMASSERT(pDestination);
	pDestination->x = V.vector4_f32[0];
    pDestination->y = V.vector4_f32[1];
    pDestination->z = V.vector4_f32[2];
}

inline XMVECTOR XMLoadFloat4(const vec4* pDestination) {    
    return vec42XMVECTOR(pDestination);
}

inline XMVECTOR XMLoadFloat3(const vec* pDestination) {    
	return vec2XMVECTOR(pDestination);
}

inline XMVECTOR XMLoadFloat3(const D3DXVECTOR3* pDestination) {    
    XMVECTOR V;
    XMASSERT(pDestination);

    ((UINT *)(&V.vector4_f32[0]))[0] = ((const UINT *)(&pDestination->x))[0];
    ((UINT *)(&V.vector4_f32[1]))[0] = ((const UINT *)(&pDestination->y))[0];
    ((UINT *)(&V.vector4_f32[2]))[0] = ((const UINT *)(&pDestination->z))[0];
    return V;
}

inline void XMStoreFloat3( D3DXVECTOR3* pDestination, FXMVECTOR V) {    
    XMASSERT(pDestination);
	pDestination->x = V.vector4_f32[0];
    pDestination->y = V.vector4_f32[1];
    pDestination->z = V.vector4_f32[2];
}



#endif