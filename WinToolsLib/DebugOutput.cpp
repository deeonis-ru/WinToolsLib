#include "DebugOutput.h"
#include "String.h"
#include <Windows.h>

namespace WinToolsLib
{
	Void DebugOutput(const TChar* format, ...)
	{
		va_list argList;
		va_start(argList, format);

		String buffer;
		buffer.FormatV(format, argList);

		if (!buffer.EndsWith(Text("\n")))
		{
			buffer += Text("\n");
		}
		::OutputDebugString(buffer);
	}
}