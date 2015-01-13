#include "String.h"
#include <string.h>
#include <mbstring.h>
#include <Windows.h>

namespace WinToolsLib
{
	//
	// ASCII -> UTF16
	//

	template <> template <>
	StringT<WChar, CharTraits<WChar>> StringT<WChar, CharTraits<WChar>>::Convert(const StringT<Char, CharTraits<Char>>& other)
	{
		auto length = ::MultiByteToWideChar(CP_ACP, 0, (PCChar)other, -1, NULL, 0);
		
		StringT<WChar, CharTraits<WChar>> result;
		result.Resize(length - 1);

		::MultiByteToWideChar(CP_ACP, 0, (PCChar)other, -1, result.GetBuffer(), length);
		return result;
	}

	//
	// UTF16 -> ASCII
	//
	
	template <> template <>
	StringT<Char, CharTraits<Char>> StringT<Char, CharTraits<Char>>::Convert(const StringT<WChar, CharTraits<WChar>>& other)
	{
		auto length = ::WideCharToMultiByte(CP_ACP, 0, (PCWChar)other, -1, NULL, 0, NULL, NULL);
		
		StringT<Char, CharTraits<Char>> result;
		result.Resize(length - 1);
		
		::WideCharToMultiByte(CP_ACP, 0, (PCWChar)other, -1, result.GetBuffer(), length, NULL, NULL);
		return result;
	}

	//
	// UTF8 -> UTF16
	//

	template <> template <>
	StringT<WChar, CharTraits<WChar>> StringT<WChar, CharTraits<WChar>>::Convert(const StringT<Byte, CharTraits<Byte>>& other)
	{
		auto length = ::MultiByteToWideChar(CP_UTF8, 0, (PCChar)(PCByte)other, -1, NULL, 0);
		
		StringT<WChar, CharTraits<WChar>> result;
		result.Resize(length - 1);

		::MultiByteToWideChar(CP_UTF8, 0, (PCChar)(PCByte)other, -1, result.GetBuffer(), length);
		return result;
	}

	//
	// UTF16 -> UTF8
	//

	template <> template <>
	StringT<Byte, CharTraits<Byte>> StringT<Byte, CharTraits<Byte>>::Convert(const StringT<WChar, CharTraits<WChar>>& other)
	{
		auto length = ::WideCharToMultiByte(CP_UTF8, 0, (PCWChar)other, -1, NULL, 0, NULL, NULL);
		
		StringT<Byte, CharTraits<Byte>> result;
		result.Resize(length - 1);
		
		::WideCharToMultiByte(CP_UTF8, 0, (PCWChar)other, -1, (PChar)result.GetBuffer(), length, NULL, NULL);
		return result;
	}

	//
	// UTF8 -> ASCII
	//
	
	template <> template <>
	StringT<Char, CharTraits<Char>> StringT<Char, CharTraits<Char>>::Convert(const StringT<Byte, CharTraits<Byte>>& other)
	{
		auto wide = StringT<WChar, CharTraits<WChar>>::Convert(other);
		return StringT<Char, CharTraits<Char>>::Convert(wide);
	}

	//
	// ASCII -> UTF8
	//

	template <> template <>
	StringT<Byte, CharTraits<Byte>> StringT<Byte, CharTraits<Byte>>::Convert(const StringT<Char, CharTraits<Char>>& other)
	{
		auto wide = StringT<WChar, CharTraits<WChar>>::Convert(other);
		return StringT<Byte, CharTraits<Byte>>::Convert(wide);
	}

	const Char CharTraits<Char>::star = '*';
	const Char CharTraits<Char>::question = '?';
	
	const Char* const CharTraits<Char>::formatInt32 = "%i";
	const Char* const CharTraits<Char>::formatInt64 = "%lli";
	const Char* const CharTraits<Char>::formatUInt32 = "%u";
	const Char* const CharTraits<Char>::formatUInt64 = "%llu";
	const Char* const CharTraits<Char>::formatDouble = "%f";

	SizeT CharTraits<Char>::GetLength(const Char* ptr)
	{
		if (nullptr == ptr)
		{
			return 0;
		}
		return strlen(ptr);
	}

	SizeT CharTraits<Char>::CountPrintF(const Elem* format, va_list argList)
	{
		return _vscprintf(format, argList);
	}

	SizeT CharTraits<Char>::PrintF(Elem* buffer, SizeT size, const Elem* format, va_list argList)
	{
		return vsprintf_s(buffer, size, format, argList);
	}

	const Byte CharTraits<Byte>::star = '*';
	const Byte CharTraits<Byte>::question = '?';
	const Byte* const CharTraits<Byte>::formatInt32 = (const Byte*)"%i";
	const Byte* const CharTraits<Byte>::formatInt64 = (const Byte*)"%lli";
	const Byte* const CharTraits<Byte>::formatUInt32 = (const Byte*)"%u";
	const Byte* const CharTraits<Byte>::formatUInt64 = (const Byte*)"%llu";
	const Byte* const CharTraits<Byte>::formatDouble = (const Byte*)"%f";

	SizeT CharTraits<Byte>::GetLength(const Byte* ptr)
	{
		if (nullptr == ptr)
		{
			return 0;
		}
		return _mbslen(ptr);
	}

	SizeT CharTraits<Byte>::CountPrintF(const Elem* format, va_list argList)
	{
		return _vscprintf((const Char*)format, argList);
	}

	SizeT CharTraits<Byte>::PrintF(Elem* buffer, SizeT size, const Elem* format, va_list argList)
	{
		return vsprintf_s((Char*)buffer, size, (const Char*)format, argList);
	}

	const WChar CharTraits<WChar>::star = L'*';
	const WChar CharTraits<WChar>::question = L'?';
	const WChar* const CharTraits<WChar>::formatInt32 = L"%i";
	const WChar* const CharTraits<WChar>::formatInt64 = L"%lli";
	const WChar* const CharTraits<WChar>::formatUInt32 = L"%u";
	const WChar* const CharTraits<WChar>::formatUInt64 = L"%llu";
	const WChar* const CharTraits<WChar>::formatDouble = L"%f";

	SizeT CharTraits<WChar>::GetLength(const WChar* ptr)
	{
		if (nullptr == ptr)
		{
			return 0;
		}
		return wcslen(ptr);
	}

	SizeT CharTraits<WChar>::CountPrintF(const Elem* format, va_list argList)
	{
		return _vscwprintf(format, argList);
	}

	SizeT CharTraits<WChar>::PrintF(Elem* buffer, SizeT size, const Elem* format, va_list argList)
	{
		return vswprintf_s(buffer, size, format, argList);
	}
}
