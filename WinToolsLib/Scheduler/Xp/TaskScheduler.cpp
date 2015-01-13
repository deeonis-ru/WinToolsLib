#include "TaskScheduler.h"

#include "..\..\Exception.h"

namespace WinToolsLib { namespace Scheduler { namespace Xp
{
	TaskScheduler::TaskScheduler()
	{
		auto hr = ::CoCreateInstance(
			CLSID_CTaskScheduler,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_ITaskScheduler,
			(PVoid*)&m_scheduler);

		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	TaskScheduler::~TaskScheduler()
	{
	}

	Task TaskScheduler::GetTask(const WChar* name) const
	{
		Com::Ptr<ITask> task;

		auto hr = m_scheduler->Activate(
			name,
			IID_ITask,
			(IUnknown**)&task);

		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Task(std::move(task));
	}

	Task TaskScheduler::CreateTask(const WChar* name)
	{
		Com::Ptr<ITask> task;

		auto hr = m_scheduler->NewWorkItem(
			name,
			CLSID_CTask,
			IID_ITask,
			(IUnknown**)&task);

		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
				
		return Task(std::move(task));
	}

	Void TaskScheduler::DeleteTask(const WChar* name)
	{
		auto hr = m_scheduler->Delete(name);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void TaskScheduler::EnumTasks(EnumCallback callback)
	{
		Com::Ptr<IEnumWorkItems> enumItems;

		auto hr = m_scheduler->Enum(&enumItems);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		while (True)
		{
			const auto fetchBy = 5ul;
			auto fetchedTasks = 0ul;
			PWChar* names = nullptr;

			hr = enumItems->Next(fetchBy, &names, &fetchedTasks);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}

			for (auto next = 0ul; next < fetchedTasks; next++)
			{
				callback(names[next]);
				::CoTaskMemFree(names[next]);
			}
			::CoTaskMemFree(names);

			if (S_FALSE == hr)
			{
				break;
			}
		}
	}

} } }