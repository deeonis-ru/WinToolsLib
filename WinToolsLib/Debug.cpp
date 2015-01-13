#include "Debug.h"
#include "Exception.h"
#include <Windows.h>

namespace WinToolsLib
{
	Debug::Debug(UInt32 processId) :
		m_processId(processId)
	{
		BOOL success = ::DebugActiveProcess(m_processId);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Debug::~Debug()
	{
		BOOL success = ::DebugActiveProcessStop(m_processId);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void Debug::KillProcessOnExit(Bool kill)
	{
		BOOL success = ::DebugSetProcessKillOnExit(kill ? TRUE : FALSE);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}
}