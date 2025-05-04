// XTCustomizeAPI.h APIs used in toolbar customization
//
// This m_file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#if !defined(__XTREGISTRYBLOB_H__)
#define __XTREGISTRYBLOB_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// A helper to load binary m_data from the registry
class CXTRegistryBlob
{
	LPBYTE m_bytes;
	LPVOID m_data;
	UINT m_nSize;
	CMemFile m_file;
	CXTCrc32 m_crc;

public:
	CXTRegistryBlob()
	: m_data(0)
	, m_nSize(0)
	, m_bytes(0)
	{	}

	~CXTRegistryBlob()
	{
		Cleanup();	
	}

	void Cleanup()
	{
		delete [] m_bytes;
		m_bytes = 0;
		free(m_data);
		m_data = 0;
	}

	DWORD ComputeCrc32(LPVOID pvData, UINT cbSize)
	{
		m_crc.SetCrc32(0);
		m_crc.Compute(pvData, cbSize);
		return m_crc.GetCrc32();		
	}

	bool Load(LPCTSTR lpszProfileName, LPCTSTR lpszEntry)
	{
		Cleanup();

		CXTRegistryManager regManager;
		if (!regManager.GetProfileBinary(lpszProfileName, lpszEntry, &m_bytes, &m_nSize))
		{
			return false;
		}

		if (m_nSize < sizeof(DWORD))
		{
			// less m_data then expected
			return false;
		}
		
		DWORD realCheckSum = ComputeCrc32(m_bytes + sizeof(DWORD), m_nSize - sizeof(DWORD));
		DWORD expectedCheckSum; 
		memcpy(&expectedCheckSum, m_bytes, sizeof(DWORD));
		if (realCheckSum != expectedCheckSum)
		{
			// data tampered with
			return false;
		}

		// data now readable through the attached file
		m_file.Attach(m_bytes + sizeof(DWORD), m_nSize - sizeof(DWORD));
		ASSERT(m_file.GetLength() == m_nSize - sizeof(DWORD));		
		return true;
	}

	bool Save(LPCTSTR lpszProfileName, LPCTSTR lpszEntry)
	{
		m_nSize = ( UINT )m_file.GetLength();
		m_data = m_file.Detach();
		DWORD checkSum = ComputeCrc32(m_data, m_nSize);
		// allocate and fill the buffer to store in registry
		LPVOID buffer = _alloca(m_nSize + sizeof(DWORD));
		memcpy(buffer, &checkSum, sizeof(DWORD));
		memcpy((LPBYTE)buffer + sizeof(DWORD), m_data, m_nSize);

		//store in the registry
		CXTRegistryManager regManager;
		return regManager.WriteProfileBinary(lpszProfileName, lpszEntry, 
			(LPBYTE)buffer, m_nSize + sizeof(DWORD)) != 0;
	}

	CMemFile* GetFile() 
	{
		return &m_file;
	}
};

#endif 
//__XTREGISTRYBLOB_H__
