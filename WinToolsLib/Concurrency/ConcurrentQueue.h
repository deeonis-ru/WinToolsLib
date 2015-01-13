#pragma once
#include "..\Types.h"
#include "..\Event.h"

#include <mutex>
#include <list>

namespace WinToolsLib { namespace Concurrency
{
	template <class T>
	class ConcurrentQueue
	{
	public:
		ConcurrentQueue();

		Void PushBack(T&& t);
		Void PushFront(T&& t);
		T Pop();

		Void Clear();
		Bool IsEmpty() const;

	private:
		mutable std::mutex m_lock;
		std::list<T> m_list;
		Event m_ready;
	};

	template <class T>
	ConcurrentQueue<T>::ConcurrentQueue()
	{
		m_ready.Create(False, False);
	}

	template <class T>
	Void ConcurrentQueue<T>::PushBack(T&& t)
	{
		std::lock_guard<std::mutex> guard(m_lock);
		m_list.push_back(std::forward<T>(t));
		m_ready.Set();
	}

	template <class T>
	Void ConcurrentQueue<T>::PushFront(T&& t)
	{
		std::lock_guard<std::mutex> guard(m_lock);
		m_list.push_front(std::forward<T>(t));
		m_ready.Set();
	}

	template <class T>
	T ConcurrentQueue<T>::Pop()
	{
		Bool haveToWait = False;
		while (True)
		{
			if (haveToWait)
			{
				m_ready.Wait();
			}

			std::lock_guard<std::mutex> guard(m_lock);
			if (m_list.empty())
			{
				haveToWait = True;
				continue;
			}

			auto t = m_list.front();
			m_list.pop_front();

			if (m_list.empty())
			{
				m_ready.Reset();
			}
			return t;
		}

		return T();
	}

	template <class T>
	Void ConcurrentQueue<T>::Clear()
	{
		std::lock_guard<std::mutex> guard(m_lock);
		m_list.clear();
		m_ready.Reset();
	}

	template <class T>
	Bool ConcurrentQueue<T>::IsEmpty() const
	{
		std::lock_guard<std::mutex> guard(m_lock);
		if (m_list.empty())
		{
			return True;
		}
		return False;
	}

} }