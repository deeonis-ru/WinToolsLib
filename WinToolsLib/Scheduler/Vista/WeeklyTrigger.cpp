#include "WeeklyTrigger.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	WeeklyTrigger::WeeklyTrigger(Com::Ptr<IWeeklyTrigger>&& trigger)
	{
		m_trigger = std::move(trigger);
	}

	WeeklyTrigger::WeeklyTrigger(WeeklyTrigger&& other)
	{
		MoveFrom(other);
	}

	WeeklyTrigger::~WeeklyTrigger()
	{
	}

	WeeklyTrigger& WeeklyTrigger::operator=(WeeklyTrigger&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Int16 WeeklyTrigger::GetWeeksInterval() const
	{
		Int16 weeks = 0;

		auto hr = m_trigger->get_WeeksInterval(&weeks);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return weeks;
	}

	Void WeeklyTrigger::SetWeeksInterval(Int16 weeks)
	{
		auto hr = m_trigger->put_WeeksInterval(weeks);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Int16 WeeklyTrigger::GetDaysOfWeek() const
	{
		Int16 days = 0;

		auto hr = m_trigger->get_DaysOfWeek(&days);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return days;
	}

	Void WeeklyTrigger::SetDaysOfWeek(Int16 days)
	{
		auto hr = m_trigger->put_DaysOfWeek(days);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void WeeklyTrigger::MoveFrom(WeeklyTrigger& other)
	{
		if (this != &other)
		{
			m_trigger = std::move(other.m_trigger);
		}
	}

} } }