#include "FirewallPolicy.h"
#include "..\..\Exception.h"

namespace WinToolsLib { namespace Os { namespace Firewall
{
	FirewallPolicy::FirewallPolicy()
	{
		auto hr = ::CoCreateInstance(
			__uuidof(NetFwPolicy2),
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwPolicy2),
			(PVoid*)&m_fwPolicy);

		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	FirewallPolicy::~FirewallPolicy()
	{
	}

	RuleList FirewallPolicy::GetRuleList()
	{
		Com::Ptr<INetFwRules> ruleList;
		auto hr = m_fwPolicy->get_Rules(&ruleList);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return RuleList(std::move(ruleList));
	}

	ProfileList FirewallPolicy::GetCurrentProfileTypes() const
	{
		long profileBitmask = 0;
		auto hr = m_fwPolicy->get_CurrentProfileTypes(&profileBitmask);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		if ((profileBitmask & (UInt32)Profile::All) == (UInt32)Profile::All)
		{
			return ProfileList({ Profile::All });
		}

		ProfileList list;
		if ((profileBitmask & (UInt32)Profile::Domain) == (UInt32)Profile::Domain) list.push_back(Profile::Domain);
		if ((profileBitmask & (UInt32)Profile::Public) == (UInt32)Profile::Public) list.push_back(Profile::Public);
		if ((profileBitmask & (UInt32)Profile::Private) == (UInt32)Profile::Private) list.push_back(Profile::Private);
		return list;
	}

	Void FirewallPolicy::SetDefaultOutboundAction(Profile profile, Action action)
	{
		auto hr = m_fwPolicy->put_DefaultOutboundAction(NET_FW_PROFILE_TYPE2(profile), NET_FW_ACTION(action));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}
} } }