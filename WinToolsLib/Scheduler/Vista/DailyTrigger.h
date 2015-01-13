#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	class DailyTrigger
	{
		DailyTrigger(const DailyTrigger& other);            // Non-copyable
		DailyTrigger& operator=(const DailyTrigger& other); // Non-copyable

	public:
		DailyTrigger(Com::Ptr<IDailyTrigger>&& trigger);
		DailyTrigger(DailyTrigger&& other);
		~DailyTrigger();

		DailyTrigger& operator=(DailyTrigger&& other);

		Int16 GetDaysInterval() const;
		Void SetDaysInterval(Int16 days);

	protected:
		Void MoveFrom(DailyTrigger& other);

	private:
		Com::Ptr<IDailyTrigger> m_trigger;
	};

} } }