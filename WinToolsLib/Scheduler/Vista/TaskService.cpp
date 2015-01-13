#include "TaskService.h"

#include "..\..\Exception.h"
#include "..\..\Com\Variant.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	TaskService::TaskService()
	{
		auto hr = ::CoCreateInstance(
			CLSID_TaskScheduler,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_ITaskService,
			(PVoid*)&m_service);

		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = m_service->Connect(
			Com::Variant(),
			Com::Variant(),
			Com::Variant(),
			Com::Variant());

		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	TaskService::~TaskService()
	{
	}

	TaskDefinition TaskService::CreateTask()
	{
		Com::Ptr<ITaskDefinition> task;
		
		auto hr = m_service->NewTask(0, &task);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return TaskDefinition(std::move(task));
	}

	TaskFolder TaskService::GetFolder(const WChar* path) const
	{
		Com::Ptr<ITaskFolder> folder;
		
		auto hr = m_service->GetFolder(_bstr_t(path), &folder);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return TaskFolder(std::move(folder));
	}

} } }