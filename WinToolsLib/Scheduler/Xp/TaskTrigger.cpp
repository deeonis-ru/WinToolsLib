#include "TaskTrigger.h"

#include "..\..\Exception.h"
#include "..\..\Time.h"

namespace WinToolsLib { namespace Scheduler { namespace Xp
{
	TaskTrigger::TaskTrigger()
	{
		ZeroMemory(&m_trigger, sizeof(m_trigger));
		m_trigger.cbTriggerSize = sizeof(m_trigger);
	}

	TriggerType TaskTrigger::GetType() const
	{
		return static_cast<TriggerType>(m_trigger.TriggerType);
	}

	Void TaskTrigger::SetType(TriggerType type)
	{
		m_trigger.TriggerType = static_cast<TASK_TRIGGER_TYPE>(type);
	}

	StringW TaskTrigger::GetStartTime() const
	{
		Time time(
			m_trigger.wBeginYear,
			m_trigger.wBeginMonth,
			m_trigger.wBeginDay,
			m_trigger.wStartHour,
			m_trigger.wStartMinute);
		
		return time.ToString(Time::IsoDateTime);
	}

	Void TaskTrigger::SetStartTime(const WChar* startTime)
	{
		auto time = Time::FromIsoDateTime(startTime);
		
		m_trigger.wBeginYear = time.GetYear();
		m_trigger.wBeginMonth = time.GetMonth();
		m_trigger.wBeginDay = time.GetDay();
		m_trigger.wStartHour = time.GetHour();
		m_trigger.wStartMinute = time.GetMinute();
	}

	UInt32 TaskTrigger::GetRepetitionDuration() const
	{
		return m_trigger.MinutesDuration;
	}

	Void TaskTrigger::SetRepetitionDuration(UInt32 minutes)
	{
		m_trigger.MinutesDuration = minutes;
	}

	UInt32 TaskTrigger::GetRepetitionInterval() const
	{
		return m_trigger.MinutesInterval;
	}

	Void TaskTrigger::SetRepetitionInterval(UInt32 minutes)
	{
		m_trigger.MinutesInterval = minutes;
	}

	UInt16 TaskTrigger::GetDaysInterval() const
	{
		return m_trigger.Type.Daily.DaysInterval;
	}

	Void TaskTrigger::SetDaysInterval(UInt16 days)
	{
		m_trigger.Type.Daily.DaysInterval = days;	
	}

	UInt16 TaskTrigger::GetWeeksInterval() const
	{
		return m_trigger.Type.Weekly.WeeksInterval;
	}

	Void TaskTrigger::SetWeeksInterval(UInt16 weeks)
	{
		m_trigger.Type.Weekly.WeeksInterval = weeks;
	}

	UInt16 TaskTrigger::GetDaysOfWeek() const
	{
		return m_trigger.Type.Weekly.rgfDaysOfTheWeek;
	}

	Void TaskTrigger::SetDaysOfWeek(UInt16 days)
	{
		m_trigger.Type.Weekly.rgfDaysOfTheWeek = days;
	}

} } }