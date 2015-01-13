#include "Application.h"
#include "Exception.h"
#include "FileSystem\File.h"
#include <Windows.h>

namespace WinToolsLib
{
	using namespace FileSystem;

	Buffer Application::LoadResource(const TChar* resName, const TChar* resType)
	{
		HMODULE hModule = ::GetModuleHandle(NULL);

		HRSRC hResInfo = ::FindResource(hModule, resName, resType); 
		if (!hResInfo)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		HGLOBAL hResData = ::LoadResource(hModule, hResInfo);
		if (!hResData)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		DWORD resSize = ::SizeofResource(hModule, hResInfo);
		LPBYTE pResRaw = (LPBYTE)::LockResource(hResData);

		return Buffer(pResRaw, resSize);
	}

	Void Application::ExtractResource(const TChar* path, const TChar* resName, const TChar* resType)
	{
		auto file = File::Create(path);
		auto resource = Application::LoadResource(resName, resType);
		file.Write(resource);
	}

	String Application::LoadStringResource(UInt32 id)
	{
		HMODULE hModule = ::GetModuleHandle(NULL);
		PTChar pBuffer = NULL;
		
		auto size = ::LoadString(hModule, id, (PTChar)&pBuffer, 0);
		if (!size)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return String(pBuffer, size);
	}
}