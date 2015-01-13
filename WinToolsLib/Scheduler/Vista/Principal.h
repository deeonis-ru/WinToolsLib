#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	enum class RunLevel
	{
		LeastPrivileges   = TASK_RUNLEVEL_LUA,
		HighestPrivileges = TASK_RUNLEVEL_HIGHEST
	};

	enum class LogonType
	{
		None                  = TASK_LOGON_NONE,
		Password              = TASK_LOGON_PASSWORD,
		ServiceForUser        = TASK_LOGON_S4U,
		Interactive           = TASK_LOGON_INTERACTIVE_TOKEN,
		Group                 = TASK_LOGON_GROUP,
		Service               = TASK_LOGON_SERVICE_ACCOUNT,
		InteractiveOrPassword = TASK_LOGON_INTERACTIVE_TOKEN_OR_PASSWORD
	};

	class Principal
	{
		Principal(const Principal& other);            // Non-copyable
		Principal& operator=(const Principal& other); // Non-copyable

	public:
		Principal();
		Principal(Com::Ptr<IPrincipal>&& principal);
		Principal(Principal&& other);
		~Principal();

		Principal& operator=(Principal&& other);
		operator IPrincipal*() const;

		RunLevel GetRunLevel() const;
		Void SetRunLevel(RunLevel runLevel);

		LogonType GetLogonType() const;
		Void SetLogonType(LogonType logonType);

		StringW GetUserId() const;
		Void SetUserId(const WChar* userId);

		StringW GetGroupId() const;
		Void SetGroupId(const WChar* groupId);

		StringW GetDisplayName() const;
		Void SetDisplayName(const WChar* displayName);

	protected:
		Void MoveFrom(Principal& other);

	private:
		Com::Ptr<IPrincipal> m_principal;
	};

	inline Principal::operator IPrincipal*() const
	{
		return m_principal.operator IPrincipal*();
	}

} } }