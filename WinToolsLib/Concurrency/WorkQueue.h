#pragma once
#include "..\Types.h"
#include "ConcurrentQueue.h"

#include <thread>
#include <functional>

namespace WinToolsLib { namespace Concurrency
{
	class WorkQueue
	{
	public:
		WorkQueue();
		~WorkQueue();

		typedef std::function<Void(Void)> WorkItem;

		Void Clear();
		Void Exit();

		Void PushBack(WorkItem workItem);
		Void PushFront(WorkItem workItem);

	protected:
		ConcurrentQueue<WorkItem> m_queue;
		Bool m_done; // NOTE: m_done MUST be declared before m_thread
		std::thread m_thread;
	};

	inline Void WorkQueue::Clear()
	{
		m_queue.PushFront([=]()
		{
			m_queue.Clear();
		});
	}

	inline Void WorkQueue::Exit()
	{
		if (m_thread.joinable())
		{
			m_queue.PushBack([=]()
			{
				m_done = True;
			});
			m_thread.join();
		}
	}

	inline Void WorkQueue::PushBack(WorkItem workItem)
	{
		m_queue.PushBack(std::move(workItem));
	}

	inline Void WorkQueue::PushFront(WorkItem workItem)
	{
		m_queue.PushBack(std::move(workItem));
	}

} }