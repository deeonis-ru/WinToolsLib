#pragma once
#include "..\Types.h"

namespace WinToolsLib
{
	namespace Os
	{
		class InternetShortcut
		{
		public:
			static Void Create(const WChar* path, const WChar* url);
		};
	}
}