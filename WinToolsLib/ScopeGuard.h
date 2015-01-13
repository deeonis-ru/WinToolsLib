#pragma once
#include "Types.h"

//
// Based on ScopeGuard implementation by
// Andrei Alexandrescu and Petru Marginean
//

namespace WinToolsLib
{
	template <class Func>
	class ScopeGuard
	{
		ScopeGuard(const ScopeGuard& other);            // Non-copyable
		ScopeGuard& operator=(const ScopeGuard& other); // Non-copyable

	public:
		ScopeGuard(Func function);
		ScopeGuard(ScopeGuard&& other);
		~ScopeGuard();

		Void Dismiss();
		Void Enforce();

	private:
		Func m_function;
		Bool m_active;
	};

	template <class Func>
	ScopeGuard<Func>::ScopeGuard(Func function) :
		m_function(std::move(function)),
		m_active(True)
	{
	}

	template <class Func>
	ScopeGuard<Func>::ScopeGuard(ScopeGuard&& other) :
		m_function(std::move(other.m_function)),
		m_active(other.m_active)
	{
		other.Dismiss();
	}

	template <class Func>
	ScopeGuard<Func>::~ScopeGuard()
	{
		Enforce();
	}

	template <class Func>
	Void ScopeGuard<Func>::Dismiss()
	{
		m_active = False;
	}

	template <class Func>
	Void ScopeGuard<Func>::Enforce()
	{
		if (m_active)
		{
			m_active = False;
			m_function();
		}
	}

	template <class Func>
	ScopeGuard<Func> OnExit(Func function)
	{
		return ScopeGuard<Func>(std::move(function));
	}
}
