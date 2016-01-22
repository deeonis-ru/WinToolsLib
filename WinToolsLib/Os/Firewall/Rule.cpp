#include "Rule.h"
#include "..\..\Exception.h"
#include "..\..\Com\Variant.h"
#include "..\..\Com\BStr.h"

namespace WinToolsLib { namespace Os { namespace Firewall
{
	Rule::Rule()
	{
		auto hr = ::CoCreateInstance(
			__uuidof(NetFwRule),
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwRule),
			(PVoid*)&m_rule);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Rule::Rule(Com::Ptr<INetFwRule>&& rule)
	{
		m_rule = std::move(rule);
	}

	Rule::Rule(Rule&& other)
	{
		MoveFrom(other);
	}

	Rule::~Rule()
	{
	}

	Rule& Rule::operator=(Rule&& other)
	{
		MoveFrom(other);
		return *this;
	}

	const Action Rule::GetAction() const
	{
		NET_FW_ACTION action;
		auto hr = m_rule->get_Action(&action);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Action(action);
	}

	const Direction Rule::GetDirection() const
	{
		NET_FW_RULE_DIRECTION direction;
		auto hr = m_rule->get_Direction(&direction);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Direction(direction);
	}

	const Bool Rule::IsEnabled() const
	{
		VARIANT_BOOL enabled;
		auto hr = m_rule->get_Enabled(&enabled);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return VARIANT_FALSE == enabled ? False : True;
	}

	const WChar* Rule::GetLocalAddresses() const
	{
		// This value can be changed
		const UInt16 maxLength = 1024;
		Com::BStr str(maxLength);

		auto hr = m_rule->get_LocalAddresses(&str);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return str;
	}
	
	const WChar* Rule::GetName() const
	{
		// This value can be changed
		const UInt16 maxLength = 1024;
		Com::BStr name(maxLength);

		auto hr = m_rule->get_Name(&name);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return name;
	}

	const WChar* Rule::GetRemoteAddresses() const
	{
		// This value can be changed
		const UInt16 maxLength = 1024;
		Com::BStr str(maxLength);

		auto hr = m_rule->get_RemoteAddresses(&str);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return str;
	}

	Void Rule::SetAction(Action value)
	{
		auto hr = m_rule->put_Action(NET_FW_ACTION(value));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Rule::SetDirection(Direction value)
	{
		auto hr = m_rule->put_Direction(NET_FW_RULE_DIRECTION(value));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Rule::SetEnabled(Bool value)
	{
		auto hr = m_rule->put_Enabled(value ? VARIANT_TRUE : VARIANT_FALSE);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Rule::SetLocalAddresses(const WChar* value)
	{
		Com::BStr str(value);
		auto hr = m_rule->put_LocalAddresses(str);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Rule::SetName(const WChar* value)
	{
		Com::BStr str(value);
		auto hr = m_rule->put_Name(str);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Rule::SetRemoteAddresses(const WChar* value)
	{
		Com::BStr str(value);
		auto hr = m_rule->put_RemoteAddresses(str);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void Rule::MoveFrom(Rule& other)
	{
		if (this != &other)
		{
			m_rule = std::move(other.m_rule);
		}
	}

	Rule::operator const INetFwRule*() const
	{
		return m_rule;
	}

	Rule::operator INetFwRule*()
	{
		return m_rule;
	}
} } }
