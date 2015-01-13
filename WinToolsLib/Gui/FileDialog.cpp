#include "FileDialog.h"

#pragma comment(lib, "Comdlg32.lib")

namespace WinToolsLib
{
	namespace Gui
	{
		FileDialog::FileDialog()
		{
			ZeroMemory(&m_ofn, sizeof(m_ofn));
			ZeroMemory(m_path, sizeof(m_path));

			m_ofn.lStructSize = sizeof(m_ofn);
			m_ofn.lpstrFile = m_path;
			m_ofn.nMaxFile = MAX_PATH;
			m_ofn.Flags = static_cast<DWORD>(FileDialogFlags::Default);
		}

		Bool FileDialog::ShowSaveDialog()
		{
			return TRUE == ::GetSaveFileName(&m_ofn);
		}

		Bool FileDialog::ShowOpenDialog()
		{
			return TRUE == ::GetOpenFileName(&m_ofn);
		}
	}
}