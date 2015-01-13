#include "RegEdit.h"
#include "Key.h"
#include "..\String.h"
#include "..\Com\Ptr.h"
#include "..\Com\TaskMem.h"

#include <Shlobj.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace WinToolsLib
{
	namespace Registry
	{
		static String GetRealPath(HKEY root, UInt32 flags, const TChar* path)
		{
			String realPath(path);

			auto withRedirection = (flags & KEY_WOW64_32KEY) != 0;
			if (withRedirection &&
				HKEY_LOCAL_MACHINE == root &&
				realPath.BeginsWith(Text("SOFTWARE")))
			{
				return String(Text("SOFTWARE\\Wow6432Node")) + realPath.SubString(8);
			}

			return realPath;
		}

		Bool RegEdit::JumpTo(HKEY root, UInt32 flags, const TChar* path)
		{
			Com::TaskMem<ITEMIDLIST> pidlComputer;
			
			auto hr = ::SHGetFolderLocation(NULL, CSIDL_DRIVES, NULL, 0, &pidlComputer);
			if (FAILED(hr))
			{
				return False;
			}

			Com::Ptr<IShellFolder> parent;
			LPCITEMIDLIST pidlRelative = nullptr;

			hr = ::SHBindToParent(pidlComputer, IID_IShellFolder, (PVoid*)&parent, &pidlRelative);
			if (FAILED(hr))
			{
				return False;
			}
			
			STRRET dispName;

			hr = parent->GetDisplayNameOf(pidlRelative, SHGDN_NORMAL, &dispName);
			if (FAILED(hr))
			{
				return False;
			}

			TChar displayName[MAX_PATH] = {0};

			hr = ::StrRetToBuf(&dispName, pidlComputer, displayName, sizeof(displayName));
			if (FAILED(hr))
			{
				return False;
			}

			auto realPath = GetRealPath(root, flags, path);		
			auto entryPath = String::FormatS(
				Text("%s\\%s\\%s"),
				displayName,
				Key::RootToFullString(root),
				realPath.GetBuffer());
			
			auto key = Key::Open(HKEY_CURRENT_USER, Text("Software\\Microsoft\\Windows\\CurrentVersion\\Applets"), KEY_CREATE_SUB_KEY);
			auto regeditKey = Key::Create(key, Text("Regedit"), KEY_SET_VALUE);

			auto data  = (PByte)entryPath.GetBuffer();
			auto size = (UInt32)entryPath.GetLength() * sizeof(TChar);

			auto success = regeditKey.SetValue(Text("LastKey"), ValueType::String, data, (UInt32)size);
			if (!success)
			{
				return False;
			}

			::ShellExecute(NULL, Text("open"), Text("regedit.exe"), NULL, NULL, SW_SHOWDEFAULT);
			return True;
		}
	}
}