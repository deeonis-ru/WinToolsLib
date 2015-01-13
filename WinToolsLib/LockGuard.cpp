#include "LockGuard.h"

namespace WinToolsLib
{
	LockGuard::LockGuard(ILockable& locable) :
		m_locable(locable)
	{
		m_locable.Lock();
	}

	LockGuard::~LockGuard()
	{
		m_locable.Unlock();
	}
}