#pragma once
#include "../Types.h"

#include <mutex>

#define OSHI_SIGNALS_MTL

#if defined OSHI_SIGNALS_MTG
#	define OSHI_SIGNALS_DEFAULT_THREAD_MODE MultiThreadGlobal
#elif defined OSHI_SIGNALS_MTL
#	define OSHI_SIGNALS_DEFAULT_THREAD_MODE MultiThreadLocal
#else
#	define OSHI_SIGNALS_DEFAULT_THREAD_MODE SingleThread
#endif

namespace WinToolsLib { namespace Signals
{
	class SingleThread
	{
	public:
		SingleThread() {}

		void lock() {}
		bool try_lock() {return true;}
		void unlock() {}
	};

	class MultiThreadGlobal
	{
	public:
		MultiThreadGlobal() {}

		void lock();
		bool try_lock();
		void unlock();

	private:
		static std::mutex m_mutex;
	};

	inline void MultiThreadGlobal::lock()
	{
		m_mutex.lock();
	}

	inline bool MultiThreadGlobal::try_lock()
	{
		return m_mutex.try_lock();
	}

	inline void MultiThreadGlobal::unlock()
	{
		m_mutex.unlock();
	}

	typedef std::mutex MultiThreadLocal;
} }