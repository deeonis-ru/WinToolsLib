#include "TaskDefinition.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	TaskDefinition::TaskDefinition(Com::Ptr<ITaskDefinition>&& task)
	{
		m_task = std::move(task);
	}

	TaskDefinition::TaskDefinition(TaskDefinition&& other)
	{
		MoveFrom(other);
	}

	TaskDefinition::~TaskDefinition()
	{
	}

	TaskDefinition& TaskDefinition::operator=(TaskDefinition&& other)
	{
		MoveFrom(other);
		return *this;
	}

	StringW TaskDefinition::GetAuthor() const
	{
		Com::Ptr<IRegistrationInfo> info;
		
		auto hr = m_task->get_RegistrationInfo(&info);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		_bstr_t author;

		hr = info->get_Author(author.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return StringW(author);
	}

	Void TaskDefinition::SetAuthor(const WChar* author)
	{
		Com::Ptr<IRegistrationInfo> info;
		
		auto hr = m_task->get_RegistrationInfo(&info);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = info->put_Author(_bstr_t(author));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Action TaskDefinition::GetAction() const
	{
		Com::Ptr<IActionCollection> collection;
		
		auto hr = m_task->get_Actions(&collection);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		Com::Ptr<IAction> action;

		hr = collection->get_Item(1, &action);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Action(std::move(action));
	}

	Action TaskDefinition::CreateAction(ActionType type)
	{
		Com::Ptr<IActionCollection> collection;
		
		auto hr = m_task->get_Actions(&collection);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = collection->Clear();
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		Com::Ptr<IAction> action;

		hr = collection->Create(static_cast<TASK_ACTION_TYPE>(type), &action);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Action(std::move(action));
	}

	Trigger TaskDefinition::GetTrigger() const
	{
		Com::Ptr<ITriggerCollection> collection;
		
		auto hr = m_task->get_Triggers(&collection);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		Com::Ptr<ITrigger> trigger;

		hr = collection->get_Item(1, &trigger);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Trigger(std::move(trigger));
	}

	Trigger TaskDefinition::CreateTrigger(TriggerType type)
	{
		Com::Ptr<ITriggerCollection> collection;
		
		auto hr = m_task->get_Triggers(&collection);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = collection->Clear();
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		Com::Ptr<ITrigger> trigger;

		hr = collection->Create(static_cast<TASK_TRIGGER_TYPE2>(type), &trigger);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Trigger(std::move(trigger));
	}

	TaskSettings TaskDefinition::GetSettings() const
	{
		Com::Ptr<ITaskSettings> settings;
		
		auto hr = m_task->get_Settings(&settings);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return TaskSettings(std::move(settings));
	}

	Void TaskDefinition::SetSettings(ITaskSettings* settings)
	{
		auto hr = m_task->put_Settings(settings);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Principal TaskDefinition::GetPrincipal() const
	{
		Com::Ptr<IPrincipal> principal;
		
		auto hr = m_task->get_Principal(&principal);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Principal(std::move(principal));
	}

	Void TaskDefinition::SetPrincipal(IPrincipal* principal)
	{
		auto hr = m_task->put_Principal(principal);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void TaskDefinition::MoveFrom(TaskDefinition& other)
	{
		if (this != &other)
		{
			m_task = std::move(other.m_task);
		}
	}

} } }