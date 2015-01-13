#include "Privilege.h"
#include "Handles\Handle.h"
#include "Exception.h"
#include <Windows.h>

namespace WinToolsLib
{
	using namespace Handles;

	Void Privilege::Set(PTChar pszPrivilege, Bool enable)
	{
		HANDLE tokenHandle = NULL;
		BOOL success = ::OpenProcessToken(
			::GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES, 
			&tokenHandle);

		Handle handle = tokenHandle;
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		LUID luid = {0};
		success = ::LookupPrivilegeValue(
			NULL,
			pszPrivilege,
			&luid);

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		TOKEN_PRIVILEGES tp = {0};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = (enable) ? SE_PRIVILEGE_ENABLED : 0;

		success = ::AdjustTokenPrivileges(
			handle,
			FALSE,
			&tp,
			sizeof(tp),
			NULL,
			NULL);

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void DebugPrivilege::Enable()
	{
		Privilege::Set(SE_DEBUG_NAME, True);
	}

	Void DebugPrivilege::Disable()
	{
		Privilege::Set(SE_DEBUG_NAME, False);
	}
}