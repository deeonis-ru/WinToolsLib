#include "FolderDialog.h"
#include "..\Com\Initialize.h"
#include "..\Com\TaskMem.h"

#include <ShlObj.h>
#pragma comment(lib, "Shell32.lib")
#pragma warning(push)
#pragma warning(disable:4090)

namespace WinToolsLib
{
	namespace Gui
	{
		FolderDialog::FolderDialog(HWND hWnd) :
			m_hWnd(hWnd)
		{
		}

		Bool FolderDialog::SelectFolder()
		{
			m_path = Text("");
			Com::Initialize init(Com::Model::ApartmentThreaded);

			BROWSEINFO bi = {0};
			bi.hwndOwner = m_hWnd;
			bi.ulFlags = BIF_USENEWUI;

			Com::TaskMem<ITEMIDLIST> pidl = ::SHBrowseForFolder(&bi);
			if (nullptr != pidl)
			{
				TCHAR path[MAX_PATH];
				::SHGetPathFromIDList(pidl, path);
				
				m_path = path;
				return True;
			}

			return False;
		}
	}
}

#pragma warning(pop)