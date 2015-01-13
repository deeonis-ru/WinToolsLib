#pragma once
#include "..\Types.h"
#include "..\String.h"

#include <Windows.h>

namespace WinToolsLib
{
	namespace Gui
	{
		class MessageBox
		{
		public:
			static Void Show(HWND hWnd, const String& text, const String& caption, const UInt32 type);
		};
	}
}

