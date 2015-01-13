#include "Principal.h"

#include "..\..\Exception.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	Principal::Principal()
	{
	}

	Principal::Principal(Com::Ptr<IPrincipal>&& principal)
	{
		m_principal = std::move(principal);
	}

	Principal::Principal(Principal&& other)
	{
		MoveFrom(other);
	}
	
	Principal::~Principal()
	{
	}

	Principal& Principal::operator=(Principal&& other)
	{
		MoveFrom(other);
		return *this;
	}

	RunLevel Principal::GetRunLevel() const
	{
		TASK_RUNLEVEL_TYPE type;

		auto hr = m_principal->get_RunLevel(&type);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<RunLevel>(type);
	}

	Void Principal::SetRunLevel(RunLevel runLevel)
	{
		auto hr = m_principal->put_RunLevel(static_cast<TASK_RUNLEVEL_TYPE>(runLevel));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	LogonType Principal::GetLogonType() const
	{
		TASK_LOGON_TYPE type;

		auto hr = m_principal->get_LogonType(&type);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<LogonType>(type);
	}

	Void Principal::SetLogonType(LogonType logonType)
	{
		auto hr = m_principal->put_LogonType(static_cast<TASK_LOGON_TYPE>(logonType));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW Principal::GetUserId() const
	{
		_bstr_t userId;

		auto hr = m_principal->get_UserId(userId.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<const WChar*>(userId);
	}

	Void Principal::SetUserId(const WChar* userId)
	{
		auto hr = m_principal->put_UserId(_bstr_t(userId));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW Principal::GetGroupId() const
	{
		_bstr_t groupId;

		auto hr = m_principal->get_GroupId(groupId.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<const WChar*>(groupId);
	}

	Void Principal::SetGroupId(const WChar* groupId)
	{
		auto hr = m_principal->put_GroupId(_bstr_t(groupId));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	StringW Principal::GetDisplayName() const
	{
		_bstr_t displayName;

		auto hr = m_principal->get_DisplayName(displayName.GetAddress());
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<const WChar*>(displayName);
	}

	Void Principal::SetDisplayName(const WChar* displayName)
	{
		auto hr = m_principal->put_DisplayName(_bstr_t(displayName));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Principal::MoveFrom(Principal& other)
	{
		if (this != &other)
		{
			m_principal = std::move(other.m_principal);
		}
	}

} } }