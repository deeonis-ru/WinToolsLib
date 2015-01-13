#pragma once
#include "Types.h"
#include "LockGuard.h"
#include <Windows.h>

namespace WinToolsLib
{
	class CriticalSection : public ILockable
	{
		CriticalSection(const CriticalSection& other);            // Non-copyable
		CriticalSection& operator=(const CriticalSection& other); // Non-copyable

		CriticalSection(CriticalSection&& other);                 // Non-movable
		CriticalSection& operator=(CriticalSection&& other);      // Non-movable

	public:
		CriticalSection(UInt32 spinCount = 0);
		~CriticalSection();

		Void Lock();
		Void Unlock();

	private:
		CRITICAL_SECTION m_section;
	};
}
