#pragma once
#include <Windows.h>

#include "..\..\Types.h"
#include "..\..\String.h"

/*
Implement class with the following static function
which is argument of template class ServiceT:

static PTChar GetName();
static UInt32 GetWaitHint(Os::Win32Service::PendingType pendingType);
static UInt32 GetAcceptedControls();

static UInt32 Start(UInt32 argc, PTChar* argv);
static UInt32 Stop();
static UInt32 Pause();
static UInt32 Continue();

Alternate way is to inherit ServiceBasic class instead of 
implementation of Stop/Pause/Continue functions.

*/

namespace WinToolsLib { namespace Os { namespace Win32Service
{
	enum class PendingType
	{
		Start = SERVICE_START_PENDING,
		Stop = SERVICE_STOP_PENDING,
		Pause = SERVICE_PAUSE_PENDING,
		Continue = SERVICE_CONTINUE_PENDING
	};

	namespace Controls
	{
		enum _Types
		{
			Stop = SERVICE_ACCEPT_STOP,
			PauseContinue = SERVICE_ACCEPT_PAUSE_CONTINUE
		};
	}

	namespace Details {
		static UInt32 ServiceType = SERVICE_WIN32_OWN_PROCESS;
		Void ReportEvent(PCTChar serviceName, PCTChar function, UInt32 error);
	}

	class ServiceBasic
	{
	public:
		static UInt32 Stop() { return NO_ERROR; }
		static UInt32 Pause() { return NO_ERROR; }
		static UInt32 Continue() { return NO_ERROR; }
	};

	class Factory;

	template <class Impl>
	class ServiceT
	{
	private:
		static Void WINAPI Main(DWORD argc, LPWSTR* argv);
		static Void WINAPI CtrlHandler(DWORD controlCode);

		static Void ReportStatus(UInt32 currentState, UInt32 win32ExitCode = NO_ERROR, UInt32 waitHint = 0);
		static Void ReportEvent(PCTChar function);

	private:
		static SERVICE_STATUS_HANDLE m_statusHandle;
		static SERVICE_STATUS m_status;

		friend class Factory;
	};

	template <class Impl>
	SERVICE_STATUS_HANDLE ServiceT<Impl>::m_statusHandle = NULL;

	template <class Impl>
	SERVICE_STATUS ServiceT<Impl>::m_status = { 0 };

	template <class Impl>
	Void ServiceT<Impl>::Main(DWORD argc, LPWSTR* argv)
	{
		m_statusHandle = ::RegisterServiceCtrlHandler(Impl::GetName(), CtrlHandler);
		if (NULL == m_statusHandle)
		{
			ReportEvent(Text("RegisterServiceCtrlHandlerEx"));
			return;
		}

		m_status.dwServiceType = Details::ServiceType;
		m_status.dwServiceSpecificExitCode = 0;

		ReportStatus(SERVICE_START_PENDING, NO_ERROR, Impl::GetWaitHint(PendingType::Start));
		const auto error = Impl::Start(argc, argv);
		ReportStatus(error == NO_ERROR ? SERVICE_RUNNING : SERVICE_STOPPED, error, 0);
	}

	template <class Impl>
	Void ServiceT<Impl>::CtrlHandler(DWORD controlCode)
	{
		auto error = NO_ERROR;
		switch (controlCode)
		{
		case SERVICE_CONTROL_STOP:
			ReportStatus(SERVICE_STOP_PENDING, error, Impl::GetWaitHint(PendingType::Stop));
			error = Impl::Stop();
			ReportStatus(SERVICE_STOPPED, error, 0);
			break;
		case SERVICE_CONTROL_PAUSE:
			ReportStatus(SERVICE_PAUSE_PENDING, error, Impl::GetWaitHint(PendingType::Pause));
			error = Impl::Pause();
			ReportStatus(SERVICE_PAUSED, error, 0);
			break;
		case SERVICE_CONTROL_CONTINUE:
			ReportStatus(SERVICE_CONTINUE_PENDING, error, Impl::GetWaitHint(PendingType::Continue));
			error = Impl::Continue();
			ReportStatus(SERVICE_RUNNING, error, 0);
			break;
		default:
			break;
		}
	}

	template <class Impl>
	Void ServiceT<Impl>::ReportStatus(UInt32 currentState, UInt32 win32ExitCode, UInt32 waitHint)
	{
		static DWORD checkPoint = 1;

		// Fill in the SERVICE_STATUS structure.

		m_status.dwCurrentState = currentState;
		m_status.dwWin32ExitCode = win32ExitCode;
		m_status.dwWaitHint = waitHint;

		if (currentState == SERVICE_START_PENDING)
		{
			m_status.dwControlsAccepted = 0;
		}
		else m_status.dwControlsAccepted = Impl::GetAcceptedControls();

		if ((currentState == SERVICE_RUNNING) ||
			(currentState == SERVICE_STOPPED))
			m_status.dwCheckPoint = 0;
		else m_status.dwCheckPoint = checkPoint++;

		// Report the status of the service to the SCM.
		::SetServiceStatus(m_statusHandle, &m_status);
	}

	template <class Impl>
	Void ServiceT<Impl>::ReportEvent(PCTChar function)
	{
		const auto error = ::GetLastError();
		Details::ReportEvent(Impl::GetName(), function, error);
	}
	
} } }
