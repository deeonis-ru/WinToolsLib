#pragma once
#include <Windows.h>

#include "..\Types.h"

namespace WinToolsLib
{
	namespace Registry
	{
		class RegEdit
		{
		public:
			static Bool JumpTo(HKEY root, UInt32 flags, const TChar* path);
		};
	}
}

