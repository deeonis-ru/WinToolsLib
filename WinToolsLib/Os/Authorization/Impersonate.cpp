#include "Impersonate.h"
#include "Sid.h"
#include "..\RemoteDesktop\Session.h"
#include "..\..\Exception.h"
#include "..\..\ScopeGuard.h"

#include <Windows.h>
#pragma comment(lib, "Advapi32.lib")

#include <UserEnv.h>
#pragma comment(lib, "UserEnv.lib")

namespace WinToolsLib { namespace Os { namespace Authorization
{
	Impersonate::Impersonate()
		: m_revertToSelf(False)
	{
		if (Sid::IsSystem())
		{
			LoggedOnUser();
			m_revertToSelf = True;
			RegDisablePredefinedCache();
		}
	}

	Impersonate::~Impersonate()
	{
		if (m_revertToSelf)
			::RevertToSelf();
	}

	Void Impersonate::LoggedOnUser()
	{
		auto impersonationToken = RemoteDesktop::Session::GetActiveUserToken();

		HANDLE userToken = nullptr;
		auto closeToken = OnExit([&userToken]()
		{
			if (userToken)
			{
				::CloseHandle(userToken);
			}
		});

		auto success = ::DuplicateTokenEx(
			impersonationToken,
			MAXIMUM_ALLOWED,
			NULL,
			SecurityImpersonation,
			TokenImpersonation,
			&userToken);

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		success = ::ImpersonateLoggedOnUser(userToken);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void Impersonate::CreateProcessAsLoggedOnUser(
		const TChar* application,
		const TChar* parameters)
	{
		auto impersonationToken = RemoteDesktop::Session::GetActiveUserToken();

		HANDLE userToken = nullptr;
		auto closeToken = OnExit([&userToken]()
		{
			if (userToken)
			{
				::CloseHandle(userToken);
			}
		});

		auto success = ::DuplicateTokenEx(
			impersonationToken,
			0,
			NULL,
			SecurityIdentification,
			TokenPrimary,
			&userToken);

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		LPVOID lpEnvironment = nullptr;
		auto destroyEnvironment = OnExit([&lpEnvironment]()
		{
			if (lpEnvironment)
			{
				::DestroyEnvironmentBlock(lpEnvironment);
			}
		});

		success = ::CreateEnvironmentBlock(
			&lpEnvironment,
			userToken,
			FALSE);

		STARTUPINFO startupInfo = {0};
		startupInfo.cb = sizeof(startupInfo);
		startupInfo.lpDesktop = Text("winsta0\\default");

		PROCESS_INFORMATION processInfo = {0};
		auto closeHandle = OnExit([&processInfo]()
		{
			if (processInfo.hThread)
			{
				::CloseHandle(processInfo.hThread);
			}
			if (processInfo.hProcess)
			{
				::CloseHandle(processInfo.hProcess);
			}
		});

		String buffer(parameters ? parameters : Text(""));

		success = ::CreateProcessAsUser(
			userToken,
			application,
			buffer.GetBuffer(),
			NULL,
			NULL,
			FALSE,
			CREATE_UNICODE_ENVIRONMENT,
			lpEnvironment,
			NULL,
			&startupInfo,
			&processInfo);
		
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

} } }