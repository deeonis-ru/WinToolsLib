#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

#include "ExecAction.h"

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	enum class ActionType
	{
		Exec = TASK_ACTION_EXEC
	};

	class Action
	{
		Action(const Action& other);            // Non-copyable
		Action& operator=(const Action& other); // Non-copyable

	public:
		Action();
		Action(Com::Ptr<IAction>&& action);
		Action(Action&& other);
		~Action();

		Action& operator=(Action&& other);

		ActionType GetType() const;
		ExecAction QueryExecAction() const;

	protected:
		Void MoveFrom(Action& other);

	private:
		Com::Ptr<IAction> m_action;
	};

} } }