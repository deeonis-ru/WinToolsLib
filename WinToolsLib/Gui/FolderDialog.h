#pragma once
#include "..\Types.h"
#include "..\String.h"

#include <Windows.h>

namespace WinToolsLib
{
	namespace Gui
	{
		class FolderDialog
		{
		public:
			FolderDialog(HWND hWnd);

			Bool SelectFolder();
			const String& GetFolderPath();

		private:
			HWND   m_hWnd;
			String m_path;
		};

		inline const String& FolderDialog::GetFolderPath()
		{
			return m_path;
		}
	}
}

