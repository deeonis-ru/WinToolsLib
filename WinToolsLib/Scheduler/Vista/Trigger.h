#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

#include "DailyTrigger.h"
#include "WeeklyTrigger.h"

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	enum class TriggerType
	{
		Time = TASK_TRIGGER_TIME,
		Daily = TASK_TRIGGER_DAILY,
		Weekly = TASK_TRIGGER_WEEKLY
	};

	class Trigger
	{
		Trigger(const Trigger& other);            // Non-copyable
		Trigger& operator=(const Trigger& other); // Non-copyable

	public:
		Trigger();
		Trigger(Com::Ptr<ITrigger>&& trigger);
		Trigger(Trigger&& other);
		~Trigger();

		Trigger& operator=(Trigger&& other);

		TriggerType GetType() const;
		DailyTrigger QueryDailyTrigger() const;
		WeeklyTrigger QueryWeeklyTrigger() const;

		StringW GetStartBoundary() const;
		Void SetStartBoundary(const WChar* startBoundary);

		StringW GetRepetitionDuration() const;
		Void SetRepetitionDuration(const WChar* duration);

		StringW GetRepetitionInterval() const;
		Void SetRepetitionInterval(const WChar* interval);

	protected:
		Void MoveFrom(Trigger& other);

	private:
		Com::Ptr<ITrigger> m_trigger;
	};

} } }