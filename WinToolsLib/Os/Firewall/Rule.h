#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <netfw.h>

namespace WinToolsLib { namespace Os { namespace Firewall
{
	enum class Action
	{
		Block = NET_FW_ACTION_BLOCK,
		Allow = NET_FW_ACTION_ALLOW
	};

	enum class Direction
	{
		In = NET_FW_RULE_DIR_IN,
		Out = NET_FW_RULE_DIR_OUT
	};

	class RuleList;

	class Rule
	{
		Rule(const Rule& other) = delete;
		Rule& operator=(const Rule& other) = delete;
	public:
		Rule();
		Rule(Com::Ptr<INetFwRule>&& rule);
		Rule(Rule&& other);
		~Rule();

		Rule& operator=(Rule&& other);

		const Action GetAction() const;
		const Direction GetDirection() const;
		const Bool IsEnabled() const;
		const WChar* GetLocalAddresses() const;
		const WChar* GetName() const;
		const WChar* GetRemoteAddresses() const;

		Void SetAction(Action value);
		Void SetDirection(Direction value);
		Void SetEnabled(Bool value);
		Void SetLocalAddresses(const WChar* value);
		Void SetName(const WChar* value);
		Void SetRemoteAddresses(const WChar* value);

	protected:
		Void MoveFrom(Rule& other);

	private:
		operator const INetFwRule*() const;
		operator INetFwRule*();

	private:
		Com::Ptr<INetFwRule> m_rule;

		friend class RuleList;
	};

} } }