// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

// TODO: reference additional headers your program requires here
#include <WinToolsLib\Types.h>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<WinToolsLib::Bool>(const WinToolsLib::Bool& value)
			{
				if (WinToolsLib::True == value)
				{
					return std::wstring(L"True");
				}
				return std::wstring(L"False");
			}

			template<> static std::wstring ToString<WinToolsLib::Int64>(const WinToolsLib::Int64& value)
			{
				WinToolsLib::WChar sz[128];
				swprintf_s(sz, L"%I64d", value);
				return std::wstring(sz);
			}

			template<> static std::wstring ToString<WinToolsLib::UInt16>(const WinToolsLib::UInt16& value)
			{
				return ToString<WinToolsLib::UInt32>((WinToolsLib::UInt32)value);
			}
		}
	}
}