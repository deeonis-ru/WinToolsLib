#pragma once
#include "Types.h"

#include <memory>
#include <mutex>
#include <type_traits>

//
// Based on monitor<T> implementation by Herb Sutter
//

namespace WinToolsLib
{
	template <class T>
	class Monitor
	{
		Monitor(const Monitor& other) = delete;            // Non-copyable
		Monitor& operator=(const Monitor& other) = delete; // Non-copyable

	public:
		Monitor()
		{
			m_lock = std::make_unique<std::mutex>();
		}
		
		explicit Monitor(T&& t) :
			m_t(std::move(t))
		{
			Monitor();
		}

		Monitor(Monitor&& other)
		{
			MoveFrom(other);
		}

		Monitor& operator=(Monitor&& other)
		{
			MoveFrom(other);
			return *this;
		}

		template<typename F>
		typename std::result_of<F(T)>::type operator()(F f) const
		{
			std::lock_guard<std::mutex> hold(*m_lock);
			return f(m_t);
		}

	protected:
		Void MoveFrom(Monitor& other)
		{
			m_t = std::move(other.m_t);
			m_lock = std::move(other.m_lock);
		}

	private:
		mutable T m_t;
		mutable std::unique_ptr<std::mutex> m_lock;
	};
}
