#include "Shortcut.h"

#include <shlobj.h>

#include "..\Exception.h"
#include "..\Com\Initialize.h"
#include "..\Com\TaskMem.h"
#include "..\Com\Ptr.h"

namespace WinToolsLib
{
	namespace FileSystem
	{
		StringW Shortcut::Resolve(const TChar* shortcutPath)
		{
			Com::Initialize init(Com::Model::SingleThreaded);
			Com::Ptr<IShellLink> shellLink;

			auto hr = ::CoCreateInstance(
				CLSID_ShellLink,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IShellLink,
				(void**)&shellLink);
			
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}

			Com::Ptr<IPersistFile> persistFile;

			hr = shellLink->QueryInterface(IID_IPersistFile, (void**)&persistFile);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}

#ifndef _UNICODE
			auto widePath = StringW::Covert(shortcutPath);
#else
			auto widePath = StringW(shortcutPath);
#endif

			hr = persistFile->Load(widePath, STGM_READ);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}

			WIN32_FIND_DATA wfd;
			WChar targetPath[MAX_PATH] = {0};

			hr = shellLink->GetPath(targetPath, MAX_PATH, &wfd, SLGP_RAWPATH);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}

			return targetPath;
		}

		Bool Shortcut::IsShortcut(const String& fileName)
		{
			return fileName.EndsWith(Text(".lnk"));
		}

		Void Shortcut::Create(const WChar* shortcutPath, const WChar* targetPath)
		{
			Com::Initialize init(Com::Model::SingleThreaded);
			Com::Ptr<IShellLink> shellLink;

			auto hr = ::CoCreateInstance(
				CLSID_ShellLink,
				NULL, 
				CLSCTX_INPROC_SERVER,
				IID_IShellLink,
				(PVoid*)&shellLink); 
			
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}

			hr = shellLink->SetPath(targetPath);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}

			Com::Ptr<IPersistFile> persistFile;

			hr = shellLink->QueryInterface(IID_IPersistFile, (PVoid*)&persistFile);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}

			hr = persistFile->Save(shortcutPath, FALSE);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}
		}
	}
}