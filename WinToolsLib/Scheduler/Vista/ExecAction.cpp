#include "ExecAction.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	ExecAction::ExecAction(Com::Ptr<IExecAction>&& action)
	{
		m_action = std::move(action);
	}

	ExecAction::ExecAction(ExecAction&& other)
	{
		MoveFrom(other);
	}

	ExecAction::~ExecAction()
	{
	}

	ExecAction& ExecAction::operator=(ExecAction&& other)
	{
		MoveFrom(other);
		return *this;
	}

	StringW ExecAction::GetPath() const
	{
		_bstr_t path;

		auto hr = m_action->get_Path(path.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return StringW(path);
	}

	Void ExecAction::SetPath(const WChar* path)
	{
		auto hr = m_action->put_Path(_bstr_t(path));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW ExecAction::GetArguments() const
	{
		_bstr_t arguments;

		auto hr = m_action->get_Arguments(arguments.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return StringW(arguments);
	}

	Void ExecAction::SetArguments(const WChar* arguments)
	{
		auto hr = m_action->put_Arguments(_bstr_t(arguments));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void ExecAction::MoveFrom(ExecAction& other)
	{
		if (this != &other)
		{
			m_action = std::move(other.m_action);
		}
	}

} } }