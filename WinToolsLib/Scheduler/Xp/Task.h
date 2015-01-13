#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <MSTask.h>
#pragma comment(lib, "MSTask.lib")

#include "TaskTrigger.h"

namespace WinToolsLib { namespace Scheduler { namespace Xp
{
	struct Flags
	{
		enum
		{
			StartOnlyIfIdle        = TASK_FLAG_START_ONLY_IF_IDLE,
			KillOnIdleEnd          = TASK_FLAG_KILL_ON_IDLE_END,
			RestartOnIdleResume    = TASK_FLAG_RESTART_ON_IDLE_RESUME,
			DontStartIfOnBatteries = TASK_FLAG_DONT_START_IF_ON_BATTERIES,
			KillIfGoingOnBatteries = TASK_FLAG_KILL_IF_GOING_ON_BATTERIES
		};
	};

	class Task
	{
		Task(const Task& other);            // Non-copyable
		Task& operator=(const Task& other); // Non-copyable

	public:
		Task(Com::Ptr<ITask>&& task);
		Task(Task&& other);
		~Task();

		Task& operator=(Task&& other);

		StringW GetApplication() const;
		Void SetApplication(const WChar* application);

		StringW GetCreator() const;
		Void SetCreator(const WChar* creator);

		StringW GetParameters() const;
		Void SetParameters(const WChar* parameters);

		UInt32 GetFlags() const;
		Void SetFlags(UInt32 flags);

		Void GetTrigger(TaskTrigger& trigger) const;
		Void SetTrigger(const TaskTrigger& trigger);
		Void DeleteTrigger();

		Void GetAccountInformation(StringW& account) const;
		Void SetAccountInformation(const WChar* account, const WChar* password);

		Void Save() const;
		Void Run() const;

	protected:
		Void MoveFrom(Task& other);

	private:
		Com::Ptr<ITask> m_task;
	};

} } }