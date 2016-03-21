#include "Mutex.h"
#include "..\Exception.h"
#include <Windows.h>

namespace WinToolsLib { namespace Process
{
	Mutex::Mutex()
		: m_isExist(False)
	{
	}

	Mutex::Mutex(Mutex&& other)
		: m_handle(std::move(other.m_handle))
		, m_isExist(std::move(other.m_isExist))
	{
	}

	Mutex::~Mutex()
	{
	}

	Void Mutex::Create(Bool initialState, const TChar* name)
	{
		m_handle = ::CreateMutex(
			NULL,
			initialState,
			name);

		if (!m_handle.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		m_isExist = ::GetLastError() == ERROR_ALREADY_EXISTS;
	}

	Void Mutex::Open(Bool modifyState, Bool inheritHandle, const TChar* name)
	{
		m_handle = ::OpenMutex(
			modifyState ? MUTEX_MODIFY_STATE : SYNCHRONIZE,
			inheritHandle,
			name);

		if (!m_handle.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
		m_isExist = True;
	}

	Bool Mutex::IsValid() const
	{
		return m_handle.IsValid();
	}

	Void Mutex::Set()
	{
		if (!m_handle.IsValid())
		{
			THROW_INVALID_HANDLE_EXCEPTION();
		}

		const auto res = ::WaitForSingleObject(m_handle, INFINITE);
		if (res != WAIT_OBJECT_0)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void Mutex::Release()
	{
		if (!m_handle.IsValid())
		{
			THROW_INVALID_HANDLE_EXCEPTION();
		}

		BOOL success = ::ReleaseMutex(m_handle);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void Mutex::Wait() const
	{
		Wait(INFINITE);
	}

	Void Mutex::Wait(UInt32 milliseconds) const
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
}}