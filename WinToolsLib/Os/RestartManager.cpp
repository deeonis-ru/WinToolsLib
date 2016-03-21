#include "RestartManager.h"
#include "..\Exception.h"
#include "..\DynamicFunction.h"

#include <Rpc.h>
#include <RestartManager.h>

#pragma comment (lib, "Rstrtmgr.lib")

namespace WinToolsLib { namespace Os
{
	typedef WinToolsLib::Process::Process Process;
	RestartManager::RestartManager() 
	{
#ifdef _USING_V110_SDK71_
		
		typedef DWORD(WINAPI *PRmStartSession)(DWORD*, DWORD, WCHAR*);
		static PRmStartSession RmStartSession = nullptr;
		if (!RmStartSession)
		{
			const auto error = ::LoadLibrary(Text("Rstrtmgr.dll"));
			if (error == NULL)
				THROW_LAST_ERROR_EXCEPTION();

			RmStartSession = DynamicFunction<PRmStartSession>("Rstrtmgr", "RmStartSession");
			if (nullptr == RmStartSession)
				THROW_LAST_ERROR_EXCEPTION();
		}
#endif
		WChar sessionKey[CCH_RM_SESSION_KEY + 1] = {};
		const auto error = RmStartSession(&m_session, 0, sessionKey);
		if (error != ERROR_SUCCESS)
		{
			m_session = MAXDWORD;
			THROW_WIN32_EXCEPTION(error);
		}
	}

	RestartManager::~RestartManager()
	{
		if (m_session != MAXDWORD)
		{
#ifdef _USING_V110_SDK71_
			typedef DWORD(WINAPI *PRmEndSession)(DWORD);
			static const auto RmEndSession = DynamicFunction<PRmEndSession>("Rstrtmgr", "RmEndSession");
#endif
			RmEndSession(m_session);
		}
	}

	std::vector<Process> RestartManager::GetResourceUsedProcessList(const TChar* path)
	{
#ifdef _USING_V110_SDK71_
		typedef DWORD(WINAPI *PRmRegisterResources)(DWORD, UINT, LPCWSTR*, UINT, RM_UNIQUE_PROCESS*, UINT, LPCWSTR*);
		static const auto RmRegisterResources = DynamicFunction<PRmRegisterResources>("Rstrtmgr", "RmRegisterResources");

		typedef DWORD(WINAPI *PRmGetList)(DWORD, UINT*, UINT*, RM_PROCESS_INFO*, LPDWORD);
		static const auto RmGetList = DynamicFunction<PRmGetList>("Rstrtmgr", "RmGetList");
#endif
		auto error = RmRegisterResources(m_session, 1, &path, 0, nullptr, 0, nullptr);
		if (ERROR_SUCCESS != error)
			THROW_WIN32_EXCEPTION(error);

		DWORD reason;
		UInt32 procInfoNeeded;
		UInt32 procInfo = 1;
		std::vector<RM_PROCESS_INFO> apps(procInfo);

		while ((error = RmGetList(m_session, &procInfoNeeded, &procInfo, apps.data(), &reason)) == ERROR_MORE_DATA)
		{
			procInfo = procInfoNeeded;
			apps.resize(procInfo);
		}
		if (ERROR_SUCCESS != error)
			THROW_WIN32_EXCEPTION(error);

		if (procInfo > 0)
		{
			std::vector<Process> processList;
			processList.reserve(procInfo);
			for (const auto& info : apps)
				processList.push_back(Process(info.Process.dwProcessId));

			return processList;
		}

		return std::vector<Process>();
	}

} }