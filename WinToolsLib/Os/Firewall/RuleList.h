#pragma once
#include "..\..\Types.h"
#include "..\..\Com\Ptr.h"

#include <netfw.h>

#include <functional>

#include "Rule.h"

namespace WinToolsLib { namespace Os { namespace Firewall
{
	class RuleList
	{
		RuleList(const RuleList& other) = delete;
		RuleList& operator=(const RuleList& other) = delete;
	public:
		RuleList(Com::Ptr<INetFwRules>&& rules);
		RuleList(RuleList&& other);
		~RuleList();

		RuleList& operator=(RuleList&& other);

		Bool IsRuleExist(const WChar* name) const;
		Void Add(Rule& rule);
		Rule Item(const WChar* name) const;

		typedef std::function<Void(Rule&& rule)> EnumCallback;
		Void EnumRules(EnumCallback callback);

	protected:
		Void MoveFrom(RuleList& other);

	private:
		Com::Ptr<INetFwRules> m_rules;
	};

} } }