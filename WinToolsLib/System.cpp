#include "System.h"
#include "Handles\Handle.h"
#include "Exception.h"
#include <Windows.h>

namespace WinToolsLib
{
	Bool System::IsXpOrLater()
	{
		UInt32 major = 0;
		UInt32 minor = 0;

		System::GetVersion(major, minor);
		if (major > 5 || (major == 5 && minor >= 1))
		{
			return True;
		}
		return False;
	}

	Bool System::IsVistaOrLater()
	{
		UInt32 major = 0;
		UInt32 minor = 0;

		System::GetVersion(major, minor);
		if (major >= 6)
		{
			return True;
		}
		return False;
	}

	Bool System::IsSevenOrLater()
	{
		UInt32 major = 0;
		UInt32 minor = 0;

		System::GetVersion(major, minor);
		if (major > 6 || (major == 6 && minor >= 1))
		{
			return True;
		}
		return False;
	}

	Bool System::IsEightOrLater()
	{
		UInt32 major = 0;
		UInt32 minor = 0;

		System::GetVersion(major, minor);
		if (major > 6 || (major == 6 && minor >= 2))
		{
			return True;
		}
		return False;
	}

	Void System::GetVersion(UInt32& major, UInt32& minor)
	{
		OSVERSIONINFO version = {0};
		version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		BOOL success = ::GetVersionEx(&version);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		major = version.dwMajorVersion;
		minor = version.dwMinorVersion;
	}

	String System::GetFullVersion()
	{
		OSVERSIONINFO version = {0};
		version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		BOOL success = ::GetVersionEx(&version);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		String fullVersion;
		fullVersion.Format(
			Text("%u.%u.%u"),
			version.dwMajorVersion,
			version.dwMinorVersion,
			version.dwBuildNumber);
		return fullVersion;
	}

	String System::GetSysDir() 
	{
		TCHAR szDir[MAX_PATH] = {0};

		BOOL success = ::GetSystemDirectory(szDir, MAX_PATH);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return String(szDir);
	}

	String System::GetWinDir() 
	{
		TCHAR szDir[MAX_PATH] = {0};

		BOOL success = ::GetWindowsDirectory(szDir, MAX_PATH);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return String(szDir);
	}

	String System::GetSysWinDir() 
	{
		TCHAR szDir[MAX_PATH] = {0};

		BOOL success = ::GetSystemWindowsDirectory(szDir, MAX_PATH);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return String(szDir);
	}

	String System::GetSysWowDir() 
	{
		TCHAR szDir[MAX_PATH] = {0};

		BOOL success = ::GetSystemWow64Directory(szDir, MAX_PATH);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return String(szDir);
	}
}