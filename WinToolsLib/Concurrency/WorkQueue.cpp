#include "WorkQueue.h"

namespace WinToolsLib { namespace Concurrency
{
	WorkQueue::WorkQueue() :
		m_done(False),
		m_thread([=]()
		{
			while (!m_done)
			{
				try
				{
					m_queue.Pop()();
				}
				catch (...)
				{
					//HANDLE_EXCEPTION();
				}
			}

			// Without ExitThread() the WorkQueue class is not usable
			// as static (Singleton) due to the bug in Visual C++:
			// "std::thread::join() hangs if called after main() exits".
			::ExitThread(0);
		})
	{
	}

	WorkQueue::~WorkQueue()
	{
		Exit();
	}
} }