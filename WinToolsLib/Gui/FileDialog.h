#pragma once
#include "..\Types.h"
#include "..\String.h"

#include <Windows.h>
#include <CommDlg.h>
#include <tchar.h>

namespace WinToolsLib
{
	namespace Gui
	{
		enum class FileDialogFlags
		{
			Explorer = OFN_EXPLORER,
			FileMustExist = OFN_FILEMUSTEXIST,
			OverwritePromt = OFN_OVERWRITEPROMPT,
			Default = OFN_EXPLORER
		};

		class FileDialog
		{
		public:
			FileDialog();

			Void SetFlags(FileDialogFlags flags);
			Void SetOwner(HWND hWnd);

			Void SetFilter(const TChar* filter);
			Void SetTitle(const TChar* title);

			Void SetDefaultFileName(const TChar* fileName);
			Void SetDefaultExtension(const TChar* extension);

			Bool ShowSaveDialog();
			Bool ShowOpenDialog();

			PCWChar GetPath();

		private:
			OPENFILENAME m_ofn;
			WChar m_path[MAX_PATH];
		};

		inline Void FileDialog::SetFlags(FileDialogFlags flags)
		{
			m_ofn.Flags = static_cast<DWORD>(flags);
		}

		inline Void FileDialog::SetOwner(HWND hWnd)
		{
			m_ofn.hwndOwner = hWnd;
		}

		inline Void FileDialog::SetFilter(const TChar* filter)
		{
			m_ofn.lpstrFilter = filter;
		}

		inline Void FileDialog::SetTitle(const TChar* title)
		{
			m_ofn.lpstrTitle = title;
		}

		inline Void FileDialog::SetDefaultFileName(const TChar* fileName)
		{
			_tcsncpy_s(m_path, fileName, MAX_PATH);
		}

		inline Void FileDialog::SetDefaultExtension(const TChar* extension)
		{
			m_ofn.lpstrDefExt = extension;
		}

		inline PCWChar FileDialog::GetPath()
		{
			return m_path;
		}
	}
}

