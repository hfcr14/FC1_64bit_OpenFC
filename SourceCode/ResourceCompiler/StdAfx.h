// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define NOT_USE_CRY_MEMORY_MANAGER
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#define NOSHLWAPI

#include <platform.h>
#include <stdio.h>
#include <tchar.h>
// Windows Header Files:
#ifdef WIN64
#include "PortableString.h"
typedef CPortableString CString;
#else
#include <atlbase.h>
#include <atlstr.h>
#endif

// Standart C headers.
#include <direct.h>
#include <assert.h>

// STL headers.
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <string>

//#include <StlDbgAlloc.h>
// to make smoother transition back from cry to std namespace...
#define cry std
#define CRY_AS_STD

// emulate the facility present in the cry engine
#include "ILog.h"

#include <smartptr.h>

//////////////////////////////////////////////////////////////////////////
#include "ResComDefs.h"

#include <Cry_Math.h>
#include <primitives.h>
#include <CryHeaders.h>
#include <IPhysics.h>

#include <IRenderer.h>
#include <IShader.h>
#include <LeafBuffer.h>

//////////////////////////////////////////////////////////////////////////
// globals.
//////////////////////////////////////////////////////////////////////////
extern void MessageBoxError( const char *format,... );
// extern void Log( const char *format,... );

#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(arr) (sizeof(arr)/sizeof((arr)[0]))
#endif
