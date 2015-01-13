#pragma once
#include "..\..\Types.h"
#include "..\..\Com\Ptr.h"

#include <MSTask.h>
#pragma comment(lib, "MSTask.lib")

#include <functional>

#include "Task.h"

namespace WinToolsLib { namespace Scheduler { namespace Xp
{
	class TaskScheduler
	{
	public:
		TaskScheduler();
		~TaskScheduler();

		Task GetTask(const WChar* name) const;
		Task CreateTask(const WChar* name);
		Void DeleteTask(const WChar* name);

		typedef std::function<Void(const WChar* name)> EnumCallback;
		Void EnumTasks(EnumCallback callback);

	private:
		Com::Ptr<ITaskScheduler> m_scheduler;
	};

} } }