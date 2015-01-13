#include "InternetShortcut.h"

#include <Windows.h>
#include <IntShCut.h>

#include "..\Exception.h"
#include "..\Com\Initialize.h"
#include "..\Com\TaskMem.h"
#include "..\Com\Ptr.h"

namespace WinToolsLib { namespace Os
{
	Void InternetShortcut::Create(const WChar* path, const WChar* url)
	{
		Com::Initialize init(Com::Model::SingleThreaded);
		Com::Ptr<IUniformResourceLocator> locator;

		auto hr = ::CoCreateInstance(
			CLSID_InternetShortcut,
			NULL, 
			CLSCTX_INPROC_SERVER,
			IID_IUniformResourceLocator,
			(PVoid*)&locator); 
			
		if (FAILED(hr))
		{
			return THROW_WIN32_EXCEPTION(hr);
		}

		hr = locator->SetURL(url, TRANSLATEURL_FL_USE_DEFAULT_PROTOCOL);
		if (FAILED(hr))
		{
			return THROW_WIN32_EXCEPTION(hr);
		}

		Com::Ptr<IPersistFile> persistFile;

		hr = locator->QueryInterface(IID_IPersistFile, (PVoid*)&persistFile);
		if (FAILED(hr))
		{
			return THROW_WIN32_EXCEPTION(hr);
		}

		hr = persistFile->Save(path, FALSE);
		if (FAILED(hr))
		{
			return THROW_WIN32_EXCEPTION(hr);
		}
	}

} }