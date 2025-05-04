// XTCrc32.h : interface for the CXTCrc32 class.
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

#if !defined(__XTCRC32_H__)
#define __XTCRC32_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Summary: A helper to compute CRC32 values 
class CXTCrc32
{
	// Summary: auxiliary table 
	DWORD m_crc32Table[256];

	// Summary: current run-on crc value 
	DWORD m_crc; 

public:
	// Summary: Constructor 
	CXTCrc32();

	// Input:	buff - bytes to compute
	//			length - count of bytes
	// Summary: calculates CRC32 on bytes in provided buffer 
	void Compute(LPCVOID buff, UINT length);

	// Returns: Current crc32 value on byte arrays passed through Compute()
	// Summary: NEEDS SUMMARY 
	DWORD GetCrc32() const
	{
		return m_crc;
	}

	// Summary: sets current run-on crc value 
	void SetCrc32(DWORD crc)
	{
		m_crc = crc;
	}
};


#endif // #if !defined(__XTCRC32_H__)
