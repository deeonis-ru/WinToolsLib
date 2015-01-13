#pragma once
#include "Types.h"

#include <functional>
#include <mutex>

namespace WinToolsLib
{
	template <class T>
	class Cached
	{
	public:
		typedef std::function<T()> Initializer;

		explicit Cached(Initializer&& init) :
			Cached(T(), std::move(init))
		{
		}
		
		explicit Cached(T&& t, Initializer&& init) :
			m_t(std::move(t)),
			m_init(std::move(init))
		{
		}

		Cached(Cached&& other)
		{
			MoveFrom(other);
		}

		Cached(const Cached& other)
		{
			CopyFrom(other);
		}

		Cached& operator=(Cached&& other)
		{
			MoveFrom(other);
			return *this;
		}
		
		Cached& operator=(const Cached& other)
		{
			CopyFrom(other);
			return *this;
		}

		operator const T&() const
		{
			if (m_init)
			{
				Initializer init;
				init.swap(m_init);
				m_init = nullptr;
				m_t = init();
			}
			return m_t;
		}

	protected:
		Void MoveFrom(Cached& other)
		{
			m_t = std::move(other.m_t);
			m_init = std::move(other.m_init);
		}

		Void CopyFrom(const Cached& other)
		{
			m_t = other.m_t;
			m_init = other.m_init;
		}

	private:
		mutable T m_t;
		mutable Initializer m_init;
	};
}
