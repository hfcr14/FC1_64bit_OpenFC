////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   memoryblock.cpp
//  Version:     v1.00
//  Created:     10/10/2002 by Timur.
//  Compilers:   Visual Studio.NET
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MemoryBlock.h"

#include <zlib.h>

//////////////////////////////////////////////////////////////////////////
CMemoryBlock::CMemoryBlock()
{
	m_buffer = 0;
	m_size = 0;
	m_uncompressedSize = 0;
	m_owns = false;
}

//////////////////////////////////////////////////////////////////////////
CMemoryBlock::CMemoryBlock( const CMemoryBlock &mem )
{
	// Invoke operator=.
	*this = mem;
}

//////////////////////////////////////////////////////////////////////////
CMemoryBlock::~CMemoryBlock()
{
	Free();
}

//////////////////////////////////////////////////////////////////////////
CMemoryBlock& CMemoryBlock::operator=( const CMemoryBlock &mem )
{
	if (mem.GetSize() > 0)
	{
		// Do not reallocate.
		if (mem.GetSize() > GetSize())
		{
			if (!Allocate(mem.GetSize()))
				return *this;
		}
		Copy( mem.GetBuffer(),mem.GetSize() );
	}
	m_uncompressedSize = mem.m_uncompressedSize;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
bool CMemoryBlock::Allocate( int size,int uncompressedSize )
{
	assert( size > 0 );
	if (m_buffer)
	{
		m_buffer = realloc(m_buffer,size);
	}
	else
	{
		m_buffer = malloc(size);
	}
	if (!m_buffer)
	{
		Warning( "Warning CMemoryBlock::Allocate failed to allocate %dMb of Memory",size/(1024*1024) );
	}

	m_owns = true;
	m_size = size;
	m_uncompressedSize = uncompressedSize;
	// Check if allocation failed.
	if (m_buffer == 0)
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CMemoryBlock::Free()
{
	if (m_buffer && m_owns)
		free( m_buffer );
	m_buffer = 0;
	m_owns = false;
	m_size = 0;
	m_uncompressedSize = 0;
}

//////////////////////////////////////////////////////////////////////////
void CMemoryBlock::Copy( void *src,int size )
{
	assert( size <= m_size );
	memcpy( m_buffer,src,size );
}

//////////////////////////////////////////////////////////////////////////
void CMemoryBlock::Attach( void *buffer,int size,int uncompressedSize )
{
	Free();
	m_owns = false;
	m_buffer = buffer;
	m_size = size;
	m_uncompressedSize = uncompressedSize;
}

//////////////////////////////////////////////////////////////////////////
void CMemoryBlock::Detach()
{
	Free();
}

//////////////////////////////////////////////////////////////////////////
void CMemoryBlock::Compress( CMemoryBlock &toBlock ) const
{
	// Cannot compress to itself.
	assert( this != &toBlock );
	ulong destSize = m_size*2+128;
	CMemoryBlock temp;
	temp.Allocate( destSize );

	compress( (uchar*)temp.GetBuffer(),&destSize,(uchar*)GetBuffer(),m_size );

	toBlock.Allocate( destSize );
	toBlock.Copy( temp.GetBuffer(),destSize );
	toBlock.m_uncompressedSize = GetSize();
}

//////////////////////////////////////////////////////////////////////////
void CMemoryBlock::Uncompress( CMemoryBlock &toBlock ) const
{
	assert( this != &toBlock );
	toBlock.Allocate( m_uncompressedSize );
	toBlock.m_uncompressedSize = 0;
	ulong destSize = m_uncompressedSize;
	int result = uncompress( (uchar*)toBlock.GetBuffer(),&destSize,(uchar*)GetBuffer(),GetSize() );
	assert( result == Z_OK );
	assert( destSize == m_uncompressedSize );
}

//////////////////////////////////////////////////////////////////////////
void CMemoryBlock::Serialize( CArchive &ar )
{
	if (ar.IsLoading())
	{
		int size;
		// Loading.
		ar >> size;
		if (size != m_size)
			Allocate( size );
		m_size = size;
		ar >> m_uncompressedSize;
		ar.Read( m_buffer,m_size );
	}
	else
	{
		// Saving.
		ar << m_size;
		ar << m_uncompressedSize;
		ar.Write( m_buffer,m_size );
	}
}