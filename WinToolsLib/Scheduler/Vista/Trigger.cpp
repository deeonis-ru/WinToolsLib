#include "Trigger.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	Trigger::Trigger()
	{
	}

	Trigger::Trigger(Com::Ptr<ITrigger>&& trigger)
	{
		m_trigger = std::move(trigger);
	}

	Trigger::Trigger(Trigger&& other)
	{
		MoveFrom(other);
	}

	Trigger::~Trigger()
	{
	}

	Trigger& Trigger::operator=(Trigger&& other)
	{
		MoveFrom(other);
		return *this;
	}

	TriggerType Trigger::GetType() const
	{
		TASK_TRIGGER_TYPE2 type2;

		auto hr = m_trigger->get_Type(&type2);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<TriggerType>(type2);
	}

	DailyTrigger Trigger::QueryDailyTrigger() const
	{
		Com::Ptr<IDailyTrigger> trigger;
		
		auto hr = m_trigger->QueryInterface(&trigger);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return DailyTrigger(std::move(trigger));
	}

	WeeklyTrigger Trigger::QueryWeeklyTrigger() const
	{
		Com::Ptr<IWeeklyTrigger> trigger;
		
		auto hr = m_trigger->QueryInterface(&trigger);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return WeeklyTrigger(std::move(trigger));
	}

	StringW Trigger::GetStartBoundary() const
	{
		_bstr_t start;

		auto hr = m_trigger->get_StartBoundary(start.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return StringW(start);
	}

	Void Trigger::SetStartBoundary(const WChar* startBoundary)
	{
		auto hr = m_trigger->put_StartBoundary(_bstr_t(startBoundary));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW Trigger::GetRepetitionDuration() const
	{
		Com::Ptr<IRepetitionPattern> repetition;
		
		auto hr = m_trigger->get_Repetition(&repetition);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		_bstr_t duration;

		hr = repetition->get_Duration(duration.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return (duration.length() > 0) ? StringW(duration) : StringW();
	}

	Void Trigger::SetRepetitionDuration(const WChar* duration)
	{
		Com::Ptr<IRepetitionPattern> repetition;
		
		auto hr = m_trigger->get_Repetition(&repetition);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = repetition->put_Duration(_bstr_t(duration));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW Trigger::GetRepetitionInterval() const
	{
		Com::Ptr<IRepetitionPattern> repetition;
		
		auto hr = m_trigger->get_Repetition(&repetition);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		_bstr_t interval;

		hr = repetition->get_Interval(interval.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return (interval.length() > 0) ? StringW(interval) : StringW();
	}

	Void Trigger::SetRepetitionInterval(const WChar* interval)
	{
		Com::Ptr<IRepetitionPattern> repetition;
		
		auto hr = m_trigger->get_Repetition(&repetition);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = repetition->put_Interval(_bstr_t(interval));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Trigger::MoveFrom(Trigger& other)
	{
		if (this != &other)
		{
			m_trigger = std::move(other.m_trigger);
		}
	}

} } }