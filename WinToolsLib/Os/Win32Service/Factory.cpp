#include "Factory.h"

#include "..\..\Exception.h"

namespace WinToolsLib { namespace Os { namespace Win32Service
{
	std::vector<SERVICE_TABLE_ENTRY> Factory::m_serviceList;

	UInt32 Factory::RunRegisteredServices()
	{
		if (m_serviceList.size() > 1)
		{
			Details::ServiceType = SERVICE_WIN32_SHARE_PROCESS;
		}
		m_serviceList.push_back(SERVICE_TABLE_ENTRY({ NULL, NULL }));

		auto table = (SERVICE_TABLE_ENTRY*)m_serviceList.data();
		if (!::StartServiceCtrlDispatcher(table))
		{
			const auto& entry = m_serviceList.front();
			const auto error = ::GetLastError();
			Details::ReportEvent(entry.lpServiceName, Text("StartServiceCtrlDispatcher"), error);
			return error;
		}

		return ERROR_SUCCESS;
	}
}}}