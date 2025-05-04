// XTCrc32.cpp : implements helper for computing crc32 values
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "XTCrc32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTCrc32::CXTCrc32()
{
	m_crc = 0;
	int i,j;
    for(i = 0; i < 256; i++) 
    {
        DWORD crc = i;
        for(j=0; j < 8; j++)
        {
            crc= (crc>>1) ^ ((crc & 1) ? 0xEDB88320UL : 0);
        }
        m_crc32Table[i]=crc & 0xFFFFFFFFUL;
    }    
}

// retrieve CRC by calling Crc32 (passing data buffer, length).
void CXTCrc32::Compute(LPCVOID buff, UINT length)
{    
    const BYTE* p = (const BYTE*)buff;
    m_crc = ~m_crc;
    while (length-- > 0)
    {
        m_crc = m_crc32Table[(m_crc ^ *p++) & 0xFF] ^ ((m_crc >> 8) & 0xFFFFFFL);
    }
    m_crc = ~m_crc;
}

