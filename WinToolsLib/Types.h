#pragma once
#include "Bool.h"

namespace WinToolsLib
{
	typedef void                Void, *PVoid;
	typedef unsigned char       Byte, *PByte;

	typedef const void          *PCVoid;
	typedef const unsigned char *PCByte;

	typedef char                Char, *PChar;
	typedef wchar_t             WChar, *PWChar;

	typedef const char          *PCChar;
	typedef const wchar_t       *PCWChar;

#ifdef UNICODE
	typedef wchar_t             TChar, *PTChar;
	typedef const wchar_t       *PCTChar;
#else
	typedef char                TChar, *PTChar;
	typedef const char          *PCTChar;
#endif

#ifdef UNICODE
	#define _Text(quote)        L##quote
#else
	#define _Text(quote)        quote
#endif

	#define Text(quote)         _Text(quote)

	typedef signed char         Int8,  *PInt8;
	typedef signed short        Int16, *PInt16;
	typedef signed int          Int32, *PInt32;
	typedef signed __int64      Int64, *PInt64;

	typedef unsigned char       UInt8,  *PUInt8;
	typedef unsigned short      UInt16, *PUInt16;
	typedef unsigned int        UInt32, *PUInt32;
	typedef unsigned __int64    UInt64, *PUInt64;

#ifdef _WIN64
	typedef unsigned __int64    SizeT, *PSizeT;
#else
	typedef unsigned int        SizeT, *PSizeT;
#endif

	typedef float               Float, *PFloat;
	typedef double              Double, *PDouble;
}