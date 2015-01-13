#include "TaskSettings.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	TaskSettings::TaskSettings(Com::Ptr<ITaskSettings>&& settings)
	{
		m_settings = std::move(settings);
	}

	TaskSettings::TaskSettings(TaskSettings&& other)
	{
		MoveFrom(other);
	}

	TaskSettings::~TaskSettings()
	{
	}

	TaskSettings& TaskSettings::operator=(TaskSettings&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Bool TaskSettings::GetDisallowStartIfOnBatteries() const
	{
		_variant_t value;

		auto hr = m_settings->get_DisallowStartIfOnBatteries(&value.boolVal);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return value.boolVal != 0;
	}

	Void TaskSettings::SetDisallowStartIfOnBatteries(Bool value)
	{
		auto hr = m_settings->put_DisallowStartIfOnBatteries(_variant_t(value));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Bool TaskSettings::GetStopIfGoingOnBatteries() const
	{
		_variant_t value;

		auto hr = m_settings->get_StopIfGoingOnBatteries(&value.boolVal);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return value.boolVal != 0;
	}

	Void TaskSettings::SetStopIfGoingOnBatteries(Bool value)
	{
		auto hr = m_settings->put_StopIfGoingOnBatteries(_variant_t(value));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Bool TaskSettings::GetRunOnlyIfIdle() const
	{
		_variant_t value;

		auto hr = m_settings->get_RunOnlyIfIdle(&value.boolVal);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return value.boolVal != 0;
	}

	Void TaskSettings::SetRunOnlyIfIdle(Bool value)
	{
		auto hr = m_settings->put_RunOnlyIfIdle(_variant_t(value));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Bool TaskSettings::GetStopOnIdleEnd() const
	{
		Com::Ptr<IIdleSettings> idle;

		auto hr = m_settings->get_IdleSettings(&idle);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		_variant_t value;

		hr = idle->get_StopOnIdleEnd(&value.boolVal);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return value.boolVal != 0;
	}

	Void TaskSettings::SetStopOnIdleEnd(Bool value)
	{
		Com::Ptr<IIdleSettings> idle;

		auto hr = m_settings->get_IdleSettings(&idle);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = idle->put_StopOnIdleEnd(_variant_t(value));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = m_settings->put_IdleSettings(idle);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Bool TaskSettings::GetStartWhenAvailable() const
	{
		_variant_t value;

		auto hr = m_settings->get_StartWhenAvailable(&value.boolVal);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return value.boolVal != 0;
	}

	Void TaskSettings::SetStartWhenAvailable(Bool value)
	{
		auto hr = m_settings->put_StartWhenAvailable(_variant_t(value));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	InstancesPolicy TaskSettings::GetInstancesPolicy() const
	{
		TASK_INSTANCES_POLICY policy;

		auto hr = m_settings->get_MultipleInstances(&policy);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<InstancesPolicy>(policy);
	}
	
	Void TaskSettings::SetInstancesPolicy(InstancesPolicy policy)
	{
		auto hr = m_settings->put_MultipleInstances(static_cast<TASK_INSTANCES_POLICY>(policy));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW TaskSettings::GetExecutionTimeLimit() const
	{
		_bstr_t limit;

		auto hr = m_settings->get_ExecutionTimeLimit(limit.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return (limit.length() > 0) ? StringW(limit) : StringW();
	}

	Void TaskSettings::SetExecutionTimeLimit(const WChar* limit)
	{
		auto hr = m_settings->put_ExecutionTimeLimit(_bstr_t(limit));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void TaskSettings::MoveFrom(TaskSettings& other)
	{
		if (this != &other)
		{
			m_settings = std::move(other.m_settings);
		}
	}

} } }