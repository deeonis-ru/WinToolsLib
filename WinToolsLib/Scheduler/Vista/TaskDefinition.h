#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

#include "Action.h"
#include "Principal.h"
#include "Trigger.h"
#include "TaskSettings.h"

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	class TaskDefinition
	{
		TaskDefinition(const TaskDefinition& other);            // Non-copyable
		TaskDefinition& operator=(const TaskDefinition& other); // Non-copyable

	public:
		TaskDefinition(Com::Ptr<ITaskDefinition>&& task);
		TaskDefinition(TaskDefinition&& other);
		~TaskDefinition();

		TaskDefinition& operator=(TaskDefinition&& other);
		operator ITaskDefinition*() const;

		StringW GetAuthor() const;
		Void SetAuthor(const WChar* author);

		Action GetAction() const;
		Action CreateAction(ActionType type);

		Trigger GetTrigger() const;
		Trigger CreateTrigger(TriggerType type);

		TaskSettings GetSettings() const;
		Void SetSettings(ITaskSettings* settings);

		Principal GetPrincipal() const;
		Void SetPrincipal(IPrincipal* principal);

	protected:
		Void MoveFrom(TaskDefinition& other);

	private:
		Com::Ptr<ITaskDefinition> m_task;
	};

	inline TaskDefinition::operator ITaskDefinition*() const
	{
		return m_task.operator ITaskDefinition*();
	}

} } }