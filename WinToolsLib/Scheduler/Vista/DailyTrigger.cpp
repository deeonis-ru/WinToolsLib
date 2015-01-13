#include "DailyTrigger.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	DailyTrigger::DailyTrigger(Com::Ptr<IDailyTrigger>&& trigger)
	{
		m_trigger = std::move(trigger);
	}

	DailyTrigger::DailyTrigger(DailyTrigger&& other)
	{
		MoveFrom(other);
	}

	DailyTrigger::~DailyTrigger()
	{
	}

	DailyTrigger& DailyTrigger::operator=(DailyTrigger&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Int16 DailyTrigger::GetDaysInterval() const
	{
		Int16 days = 0;

		auto hr = m_trigger->get_DaysInterval(&days);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return days;
	}

	Void DailyTrigger::SetDaysInterval(Int16 days)
	{
		auto hr = m_trigger->put_DaysInterval(days);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void DailyTrigger::MoveFrom(DailyTrigger& other)
	{
		if (this != &other)
		{
			m_trigger = std::move(other.m_trigger);
		}
	}

} } }