#pragma once
#include "..\..\Types.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

#include <functional>

#include "RegisteredTask.h"

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	class TaskFolder
	{
		TaskFolder(const TaskFolder& other);            // Non-copyable
		TaskFolder& operator=(const TaskFolder& other); // Non-copyable

	public:
		TaskFolder(Com::Ptr<ITaskFolder>&& folder);
		TaskFolder(TaskFolder&& other);
		~TaskFolder();

		TaskFolder& operator=(TaskFolder&& other);

		TaskFolder GetFolder(const WChar* path, Bool createIfNotExist = False);

		RegisteredTask GetTask(const WChar* path);
		inline RegisteredTask RegisterTask(const WChar* path, ITaskDefinition* definition);
		inline RegisteredTask UpdateTask(const WChar* path, ITaskDefinition* definition);

		Void DeleteTask(const WChar* name);
		Void DeleteFolder(const WChar* path);

		typedef std::function<Void(const WChar* name)> EnumCallback;
		Void EnumTasks(EnumCallback callback);

	protected:
		Void MoveFrom(TaskFolder& other);

	private:
		RegisteredTask RegisterTaskImpl(const WChar* path, ITaskDefinition* definition, LONG flags);

	private:
		Com::Ptr<ITaskFolder> m_folder;
	};

	RegisteredTask TaskFolder::RegisterTask(const WChar* path, ITaskDefinition* definition)
	{
		return RegisterTaskImpl(path, definition, TASK_CREATE);
	}

	RegisteredTask TaskFolder::UpdateTask(const WChar* path, ITaskDefinition* definition)
	{
		return RegisterTaskImpl(path, definition, TASK_UPDATE);
	}

} } }