#include "MessageBox.h"

namespace WinToolsLib
{
	namespace Gui
	{
		Void MessageBox::Show(HWND hWnd, const String& text, const String& caption, const UInt32 type)
		{
			auto style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
			if ((style & WS_VISIBLE) == WS_VISIBLE)
				::MessageBox(hWnd, text, caption, type);
		}
	}
}