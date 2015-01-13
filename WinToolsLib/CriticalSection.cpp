#include "CriticalSection.h"
#include "Exception.h"

namespace WinToolsLib
{
	CriticalSection::CriticalSection(UInt32 spinCount)
	{
		auto success = ::InitializeCriticalSectionAndSpinCount(&m_section, spinCount);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	CriticalSection::~CriticalSection()
	{
		::DeleteCriticalSection(&m_section);
	}

	Void CriticalSection::Lock()
	{
		::EnterCriticalSection(&m_section);
	}

	Void CriticalSection::Unlock()
	{
		::LeaveCriticalSection(&m_section);
	}
}