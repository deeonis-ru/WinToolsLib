#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	enum class DaysOfWeek : Int16
	{
		Sunday    = 0x01,
		Monday    = 0x02,
		Tuesday   = 0x04,
		Wednesday = 0x08,
		Thursday  = 0x10,
		Friday    = 0x20,
		Saturday  = 0x40
	};

	class WeeklyTrigger
	{
		WeeklyTrigger(const WeeklyTrigger& other);            // Non-copyable
		WeeklyTrigger& operator=(const WeeklyTrigger& other); // Non-copyable

	public:
		WeeklyTrigger(Com::Ptr<IWeeklyTrigger>&& trigger);
		WeeklyTrigger(WeeklyTrigger&& other);
		~WeeklyTrigger();

		WeeklyTrigger& operator=(WeeklyTrigger&& other);

		Int16 GetWeeksInterval() const;
		Void SetWeeksInterval(Int16 weeks);

		Int16 GetDaysOfWeek() const;
		Void SetDaysOfWeek(Int16 days);

	protected:
		Void MoveFrom(WeeklyTrigger& other);

	private:
		Com::Ptr<IWeeklyTrigger> m_trigger;
	};

} } }