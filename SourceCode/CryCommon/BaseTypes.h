// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#ifndef __BASETYPES_H__
#define __BASETYPES_H__

static_assert(sizeof(char) == 1);
static_assert(sizeof(float) == 4);
static_assert(sizeof(int) >= 4);

typedef unsigned char  byte;
typedef unsigned char  ubyte;

typedef unsigned char  uchar;
typedef signed char    schar;

typedef unsigned short ushort;
typedef signed short   sshort;

#if !defined(CLANG_FIX_UINT_REDEF)
typedef unsigned int       uint;
#endif
typedef signed int         sint;

typedef unsigned long	   ulong;
typedef signed long        slong;

typedef unsigned long long ulonglong;
typedef signed long long   slonglong;

static_assert(sizeof(uchar) == sizeof(schar));
static_assert(sizeof(ushort) == sizeof(sshort));
static_assert(sizeof(uint) == sizeof(sint));
static_assert(sizeof(ulong) == sizeof(slong));
static_assert(sizeof(ulonglong) == sizeof(slonglong));

static_assert(sizeof(uchar) <= sizeof(ushort));
static_assert(sizeof(ushort) <= sizeof(uint));
static_assert(sizeof(uint) <= sizeof(ulong));
static_assert(sizeof(ulong) <= sizeof(ulonglong));

typedef schar int8;
typedef schar sint8;
typedef uchar uint8;
static_assert(sizeof(uint8) == 1);
static_assert(sizeof(sint8) == 1);

typedef sshort int16;
typedef sshort sint16;
typedef ushort uint16;
static_assert(sizeof(uint16) == 2);
static_assert(sizeof(sint16) == 2);

typedef sint int32;
typedef sint sint32;
typedef uint uint32;
static_assert(sizeof(uint32) == 4);
static_assert(sizeof(sint32) == 4);

typedef slonglong int64;
typedef slonglong sint64;
typedef ulonglong uint64;
static_assert(sizeof(uint64) == 8);
static_assert(sizeof(sint64) == 8);

typedef float  f32;
typedef double f64;
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

typedef f64		real;
typedef int		index_t;

#endif