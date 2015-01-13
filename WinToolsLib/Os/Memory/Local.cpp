#include "Local.h"
#include "..\..\Exception.h"

namespace WinToolsLib
{
	namespace Memory
	{
		Local::Local(HLOCAL hLocal) :
			m_hLocal(hLocal),
			m_pointer(nullptr)
		{
		}

		Local::~Local()
		{
			Free();
		}

		Void Local::Free()
		{
			Unlock();
			if (m_hLocal)
			{
				::LocalFree(m_hLocal);
				m_hLocal = nullptr;
			}
		}

		PVoid Local::Lock()
		{
			if (!m_pointer && m_hLocal)
			{
				m_pointer = ::LocalLock(m_hLocal);
			}
			return m_pointer;
		}

		Void Local::Unlock()
		{
			if (m_pointer && m_hLocal)
			{
				::LocalUnlock(m_hLocal);
				m_pointer = nullptr;
			}
		}

		Local Local::Alloc(UInt32 flags, SizeT bytes)
		{
			Local local(::LocalAlloc(flags, bytes));
			if (!local.m_hLocal)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
			return local;
		}
	}
}