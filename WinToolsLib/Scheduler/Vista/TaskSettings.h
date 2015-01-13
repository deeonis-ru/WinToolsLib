#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	enum class InstancesPolicy
	{
		RunInParallel = TASK_INSTANCES_PARALLEL,
		Queue         = TASK_INSTANCES_QUEUE,
		IgnoreNew     = TASK_INSTANCES_IGNORE_NEW,
		StopExisting  = TASK_INSTANCES_STOP_EXISTING
	};

	class TaskSettings
	{
		TaskSettings(const TaskSettings& other);            // Non-copyable
		TaskSettings& operator=(const TaskSettings& other); // Non-copyable

	public:
		TaskSettings(Com::Ptr<ITaskSettings>&& settings);
		TaskSettings(TaskSettings&& other);
		~TaskSettings();

		TaskSettings& operator=(TaskSettings&& other);
		operator ITaskSettings*() const;

		Bool GetDisallowStartIfOnBatteries() const;
		Void SetDisallowStartIfOnBatteries(Bool value);

		Bool GetStopIfGoingOnBatteries() const;
		Void SetStopIfGoingOnBatteries(Bool value);

		Bool GetRunOnlyIfIdle() const;
		Void SetRunOnlyIfIdle(Bool value);

		Bool GetStopOnIdleEnd() const;
		Void SetStopOnIdleEnd(Bool value);

		Bool GetStartWhenAvailable() const;
		Void SetStartWhenAvailable(Bool value);

		InstancesPolicy GetInstancesPolicy() const;
		Void SetInstancesPolicy(InstancesPolicy policy);

		StringW GetExecutionTimeLimit() const;
		Void SetExecutionTimeLimit(const WChar* limit);

	protected:
		Void MoveFrom(TaskSettings& other);

	private:
		Com::Ptr<ITaskSettings> m_settings;
	};

	inline TaskSettings::operator ITaskSettings*() const
	{
		return m_settings.operator ITaskSettings*();
	}

} } }