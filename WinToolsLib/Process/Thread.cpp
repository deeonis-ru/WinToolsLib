#include "Thread.h"
#include <Windows.h>
#include <TlHelp32.h>

namespace WinToolsLib { namespace Process
{
	Thread::Thread(UInt32 id) :
		m_id(id)
	{
		m_handle = ::OpenThread(THREAD_SUSPEND_RESUME, FALSE, m_id);
		if (m_handle.IsNull())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Thread::Thread(UInt32 id, Handle&& handle) :
		m_id(id),
		m_handle(std::move(handle))
	{
	}

	Thread::Thread(UInt32 id, const Handle& handle) :
		m_id(id),
		m_handle(handle)
	{
	}

	Thread::Thread(Handle&& handle) :
		m_id(m_invalidId),
		m_handle(std::move(handle))
	{
	}

	Thread::Thread(const Handle& handle) :
		m_id(m_invalidId),
		m_handle(handle)
	{
	}

	Thread::Thread(Thread&& other)
	{
		MoveFrom(other);
	}

	Thread::Thread(const Thread& other)
	{
		CopyFrom(other);
	}

	Thread& Thread::operator=(Thread&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Thread& Thread::operator=(const Thread& other)
	{
		CopyFrom(other);
		return *this;
	}

	Void Thread::Wait() const
	{
		Wait(INFINITE);
	}

	Void Thread::Wait(UInt32 milliseconds) const
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

	Void Thread::Suspend()
	{
		if (::SuspendThread(m_handle) == (DWORD)-1)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void Thread::Resume()
	{
		if (::ResumeThread(m_handle) == (DWORD)-1)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void Thread::Sleep(UInt32 milliseconds)
	{
		::Sleep(milliseconds);
	}

	ThreadList Thread::GetList(UInt32 processId)
	{
		Handle hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, processId);
		if (!hSnapshot.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		THREADENTRY32 threadEntry = {0};
		threadEntry.dwSize = sizeof(threadEntry);

		BOOL success = ::Thread32First(hSnapshot, &threadEntry);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		ThreadList list;
		while (success)
		{
			if (threadEntry.th32OwnerProcessID == processId)
			{
				list.push_back(Thread(threadEntry.th32ThreadID));
			}
			success = ::Thread32Next(hSnapshot, &threadEntry);
		}

		return list;
	}

	Void Thread::MoveFrom(Thread& other)
	{
		if (this != &other)
		{
			m_id = other.m_id;
			m_handle = std::move(other.m_handle);

			other.m_id = m_invalidId;
		}
	}

	Void Thread::CopyFrom(const Thread& other)
	{
		if (this != &other)
		{
			m_id = other.m_id;
			m_handle = other.m_handle;
		}
	}
}}