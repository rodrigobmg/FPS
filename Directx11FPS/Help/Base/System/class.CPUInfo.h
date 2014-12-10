//-------------------------------------------------------------------------------------
// CpuTopology.h
// 
// CpuToplogy class declaration.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------
#pragma once
#ifndef CPU_INFO_H
#define CPU_INFO_H
#include <windows.h>

//---------------------------------------------------------------------------------
// Name: CPUInfo
// Desc: This class constructs a supported cpu topology implementation object on
//       initialization and forwards calls to it.  This is the Abstraction class
//       in the traditional Bridge Pattern.
//---------------------------------------------------------------------------------
class ICPUInfo;

class CPUInfo {
public:
    CPUInfo( BOOL bForceCpuid = FALSE );
    ~CPUInfo();

    BOOL IsDefaultImpl() const;
    DWORD NumberOfProcessCores() const;
    DWORD NumberOfSystemCores() const;
    DWORD_PTR CoreAffinityMask( DWORD coreIdx ) const;
    void ForceCpuid( BOOL bForce );

private:
    void Destroy();
    ICPUInfo* m_pImpl;
};

#endif
