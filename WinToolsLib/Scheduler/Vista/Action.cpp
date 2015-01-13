#include "Action.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	Action::Action()
	{
	}

	Action::Action(Com::Ptr<IAction>&& action)
	{
		m_action = std::move(action);
	}

	Action::Action(Action&& other)
	{
		MoveFrom(other);
	}

	Action::~Action()
	{
	}

	Action& Action::operator=(Action&& other)
	{
		MoveFrom(other);
		return *this;
	}

	ActionType Action::GetType() const
	{
		TASK_ACTION_TYPE type;

		auto hr = m_action->get_Type(&type);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<ActionType>(type);
	}

	ExecAction Action::QueryExecAction() const
	{
		Com::Ptr<IExecAction> action;
		
		auto hr = m_action->QueryInterface(&action);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return ExecAction(std::move(action));
	}

	Void Action::MoveFrom(Action& other)
	{
		if (this != &other)
		{
			m_action = std::move(other.m_action);
		}
	}

} } }