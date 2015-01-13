#include "TaskFolder.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	TaskFolder::TaskFolder(Com::Ptr<ITaskFolder>&& folder)
	{
		m_folder = std::move(folder);
	}

	TaskFolder::TaskFolder(TaskFolder&& other)
	{
		MoveFrom(other);
	}

	TaskFolder::~TaskFolder()
	{
	}

	TaskFolder& TaskFolder::operator=(TaskFolder&& other)
	{
		MoveFrom(other);
		return *this;
	}

	TaskFolder TaskFolder::GetFolder(const WChar* path, Bool createIfNotExist)
	{
		Com::Ptr<ITaskFolder> folder;

		auto hr = m_folder->GetFolder(_bstr_t(path), &folder);
		if (HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) == hr && createIfNotExist)
		{
			hr = m_folder->CreateFolder(_bstr_t(path), _variant_t(), &folder);
		}

		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return TaskFolder(std::move(folder));
	}

	RegisteredTask TaskFolder::GetTask(const WChar* path)
	{
		Com::Ptr<IRegisteredTask> task;

		auto hr = m_folder->GetTask(_bstr_t(path), &task);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return RegisteredTask(std::move(task));
	}

	RegisteredTask TaskFolder::RegisterTask(const WChar* path, ITaskDefinition* definition)
	{
		Com::Ptr<IPrincipal> principal;

		auto hr = definition->get_Principal(&principal);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		_bstr_t userId;

		hr = principal->get_UserId(userId.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		TASK_LOGON_TYPE logonType;

		hr = principal->get_LogonType(&logonType);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		Com::Ptr<IRegisteredTask> task;

		hr = m_folder->RegisterTaskDefinition(
			_bstr_t(path),
			definition,
			TASK_CREATE,
			_variant_t(userId),
			_variant_t(),
			logonType,
			_variant_t(),
			&task);
		
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return RegisteredTask(std::move(task));
	}

	Void TaskFolder::DeleteTask(const WChar* name)
	{
		auto hr = m_folder->DeleteTask(_bstr_t(name), 0);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void TaskFolder::DeleteFolder(const WChar* path)
	{
		auto hr = m_folder->DeleteFolder(_bstr_t(path), 0);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void TaskFolder::EnumTasks(EnumCallback callback)
	{
		Com::Ptr<IRegisteredTaskCollection> collection;
		
		auto hr = m_folder->GetTasks(0, &collection);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		LONG count = 0;

		hr = collection->get_Count(&count);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		for (LONG i = 0; i < count; i++)
		{
			Com::Ptr<IRegisteredTask> task;

			hr = collection->get_Item(_variant_t(i + 1), &task);
			if (FAILED(hr))
			{
				continue;
			}

			_bstr_t name;

			hr = task->get_Name(name.GetAddress());
			if (FAILED(hr))
			{
				continue;
			}

			callback(name);
		}
	}

	Void TaskFolder::MoveFrom(TaskFolder& other)
	{
		if (this != &other)
		{
			m_folder = std::move(other.m_folder);
		}
	}

} } }