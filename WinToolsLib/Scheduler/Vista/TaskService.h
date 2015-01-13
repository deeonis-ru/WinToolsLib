#pragma once
#include "..\..\Types.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

#include "TaskDefinition.h"
#include "TaskFolder.h"

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	class TaskService
	{
	public:
		TaskService();
		~TaskService();

		TaskDefinition CreateTask();
		TaskFolder GetFolder(const WChar* path) const;

	private:
		Com::Ptr<ITaskService> m_service;
	};

} } }