#pragma once
#include "..\..\Types.h"
#include "..\..\Com\Ptr.h"

#include <netfw.h>
#include <vector>

#include "RuleList.h"

namespace WinToolsLib { namespace Os { namespace Firewall
{
	enum class Profile
	{
		Domain = NET_FW_PROFILE2_DOMAIN,
		Private = NET_FW_PROFILE2_PRIVATE,
		Public = NET_FW_PROFILE2_PUBLIC,
		All = NET_FW_PROFILE2_ALL
	};
	typedef std::vector<Profile> ProfileList;

	class FirewallPolicy
	{
	public:
		FirewallPolicy();
		~FirewallPolicy();

		RuleList GetRuleList();
		ProfileList GetCurrentProfileTypes() const;

		Void SetDefaultOutboundAction(Profile profile, Action action);

	private:
		Com::Ptr<INetFwPolicy2> m_fwPolicy;
	};

} } }