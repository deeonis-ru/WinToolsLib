#include "Task.h"

#include "..\..\Exception.h"
#include "..\..\Com\TaskMem.h"

namespace WinToolsLib { namespace Scheduler { namespace Xp
{
	Task::Task(Com::Ptr<ITask>&& task)
	{
		m_task = std::move(task);
	}

	Task::Task(Task&& other)
	{
		MoveFrom(other);
	}

	Task::~Task()
	{
	}

	Task& Task::operator=(Task&& other)
	{
		MoveFrom(other);
		return *this;
	}

	StringW Task::GetApplication() const
	{
		Com::TaskMem<WCHAR> application;

		auto hr = m_task->GetApplicationName(&application);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return StringW(application);
	}

	Void Task::SetApplication(const WChar* application)
	{
		auto hr = m_task->SetApplicationName(application);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW Task::GetCreator() const
	{
		Com::TaskMem<WCHAR> creator;

		auto hr = m_task->GetCreator(&creator);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return StringW(creator);
	}

	Void Task::SetCreator(const WChar* creator)
	{
		auto hr = m_task->SetCreator(creator);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW Task::GetParameters() const
	{
		Com::TaskMem<WCHAR> parameters;

		auto hr = m_task->GetParameters(&parameters);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return StringW(parameters);
	}

	Void Task::SetParameters(const WChar* parameters)
	{
		auto hr = m_task->SetParameters(parameters);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	UInt32 Task::GetFlags() const
	{
		DWORD flags = 0;

		auto hr = m_task->GetFlags(&flags);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return flags;
	}

	Void Task::SetFlags(UInt32 flags)
	{
		auto hr = m_task->SetFlags(flags);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Task::GetTrigger(TaskTrigger& data) const
	{
		Com::Ptr<ITaskTrigger> trigger;

		auto hr = m_task->GetTrigger(0, &trigger);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = trigger->GetTrigger(data.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Task::SetTrigger(const TaskTrigger& data)
	{
		Com::Ptr<ITaskTrigger> trigger;

		auto hr = m_task->GetTrigger(0, &trigger);
		if (FAILED(hr))
		{
			UInt16 index = 0;

			hr = m_task->CreateTrigger(&index, &trigger);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}
		}

		hr = trigger->SetTrigger(const_cast<const PTASK_TRIGGER>(data.GetAddress()));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Task::DeleteTrigger()
	{
		m_task->DeleteTrigger(0);
	}

	Void Task::GetAccountInformation(StringW& account) const
	{
		Com::TaskMem<WCHAR> name;

		auto hr = m_task->GetAccountInformation(&name);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		account = name;
	}

	Void Task::SetAccountInformation(const WChar* account, const WChar* password)
	{
		auto hr = m_task->SetAccountInformation(account, password);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Task::Save() const
	{
		Com::Ptr<IPersistFile> file;

		auto hr = m_task->QueryInterface(&file);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = file->Save(NULL, TRUE);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Task::Run() const
	{
		auto hr = m_task->Run();
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Task::MoveFrom(Task& other)
	{
		if (this != &other)
		{
			m_task = std::move(other.m_task);
		}
	}

} } }