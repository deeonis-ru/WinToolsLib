#include "Session.h"
#include "..\..\Exception.h"

#include <Windows.h>
#include <WtsApi32.h>
#pragma comment(lib, "WtsApi32.lib")

namespace WinToolsLib { namespace Os { namespace RemoteDesktop
{
	Handles::Handle Session::GetActiveUserToken()
	{
		auto sessionId = ::WTSGetActiveConsoleSessionId();
		HANDLE handle = nullptr;

		auto success = ::WTSQueryUserToken(sessionId, &handle);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return Handles::Handle(handle);
	}

} } }