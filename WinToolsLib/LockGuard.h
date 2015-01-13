#pragma once
#include "Types.h"
#include "ILocable.h"

namespace WinToolsLib
{
	class LockGuard
	{
		LockGuard(const LockGuard& other);            // Non-copyable
		LockGuard& operator=(const LockGuard& other); // Non-copyable

		LockGuard(LockGuard&& other);                 // Non-movable
		LockGuard& operator=(LockGuard&& other);      // Non-movable

	public:
		LockGuard(ILockable& locable);
		~LockGuard();

	private:
		ILockable& m_locable;
	};
}
