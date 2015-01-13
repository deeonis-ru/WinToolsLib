#include "RegisteredTask.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	RegisteredTask::RegisteredTask(Com::Ptr<IRegisteredTask>&& task)
	{
		m_task = std::move(task);
	}

	RegisteredTask::RegisteredTask(RegisteredTask&& other)
	{
		MoveFrom(other);
	}

	RegisteredTask::~RegisteredTask()
	{
	}

	RegisteredTask& RegisteredTask::operator=(RegisteredTask&& other)
	{
		MoveFrom(other);
		return *this;
	}

	TaskDefinition RegisteredTask::GetDefinition() const
	{
		Com::Ptr<ITaskDefinition> definition;

		auto hr = m_task->get_Definition(&definition);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return TaskDefinition(std::move(definition));
	}

	Void RegisteredTask::Run() const
	{
		auto hr = m_task->Run(_variant_t(), nullptr);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void RegisteredTask::Run(const WChar* param) const
	{
		auto hr = m_task->Run(_variant_t(_bstr_t(param)), nullptr);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void RegisteredTask::MoveFrom(RegisteredTask& other)
	{
		if (this != &other)
		{
			m_task = std::move(other.m_task);
		}
	}

} } }