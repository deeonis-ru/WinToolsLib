#include "Event.h"
#include "..\Exception.h"
#include <Windows.h>

namespace WinToolsLib { namespace Process
{
	Event::Event()
	{
	}
	
	Event::Event(Event&& other)
		: m_handle(std::move(other.m_handle))
	{
	}

	Event::~Event()
	{
	}

	Void Event::Create(Bool manualReset, Bool initialState, const TChar* name)
	{
		m_handle = ::CreateEvent(
			NULL,
			manualReset,
			initialState,
			name);
		
		if (!m_handle.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}
	
	Void Event::Open(Bool modifyState, Bool inheritHandle, const TChar* name)
	{
		m_handle = ::OpenEvent(
			modifyState ? EVENT_MODIFY_STATE : SYNCHRONIZE,
			inheritHandle,
			name);
		
		if (!m_handle.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Bool Event::IsValid() const
	{
		return m_handle.IsValid();
	}

	Void Event::Set()
	{
		if (!m_handle.IsValid())
		{
			THROW_INVALID_HANDLE_EXCEPTION();
		}

		BOOL success = ::SetEvent(m_handle);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void Event::Reset()
	{
		if (!m_handle.IsValid())
		{
			THROW_INVALID_HANDLE_EXCEPTION();
		}

		BOOL success = ::ResetEvent(m_handle);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void Event::Wait() const
	{
		Wait(INFINITE);
	}

	Void Event::Wait(UInt32 milliseconds) const
	{
		if (!m_handle.IsValid())
		{
			THROW_INVALID_HANDLE_EXCEPTION();
		}

		DWORD result = ::WaitForSingleObject(m_handle, milliseconds);
		if (WAIT_FAILED == result)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}
} }