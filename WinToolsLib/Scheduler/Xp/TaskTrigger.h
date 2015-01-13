#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <MSTask.h>
#pragma comment(lib, "MSTask.lib")

namespace WinToolsLib { namespace Scheduler { namespace Xp
{
	enum class TriggerType
	{
		Once = TASK_TIME_TRIGGER_ONCE,
		Daily = TASK_TIME_TRIGGER_DAILY,
		Weekly = TASK_TIME_TRIGGER_WEEKLY
	};

	enum class DaysOfWeek : UInt16
	{
		Sunday    = TASK_SUNDAY,
		Monday    = TASK_MONDAY,
		Tuesday   = TASK_TUESDAY,
		Wednesday = TASK_WEDNESDAY,
		Thursday  = TASK_THURSDAY,
		Friday    = TASK_FRIDAY,
		Saturday  = TASK_SATURDAY
	};

	class TaskTrigger
	{
	public:
		TaskTrigger();

		const TASK_TRIGGER* GetAddress() const;
		TASK_TRIGGER* GetAddress();

		TriggerType GetType() const;
		Void SetType(TriggerType type);

		StringW GetStartTime() const;
		Void SetStartTime(const WChar* startTime);

		UInt32 GetRepetitionDuration() const;
		Void SetRepetitionDuration(UInt32 minutes);

		UInt32 GetRepetitionInterval() const;
		Void SetRepetitionInterval(UInt32 minutes);

		UInt16 GetDaysInterval() const;
		Void SetDaysInterval(UInt16 days);

		UInt16 GetWeeksInterval() const;
		Void SetWeeksInterval(UInt16 weeks);

		UInt16 GetDaysOfWeek() const;
		Void SetDaysOfWeek(UInt16 days);

	private:
		TASK_TRIGGER m_trigger;
	};

	inline const TASK_TRIGGER* TaskTrigger::GetAddress() const
	{
		return &m_trigger;
	}

	inline TASK_TRIGGER* TaskTrigger::GetAddress()
	{
		return &m_trigger;
	}

} } }