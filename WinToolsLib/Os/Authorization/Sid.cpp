#include "Sid.h"

#pragma comment(lib, "Advapi32.lib")

namespace WinToolsLib { namespace Os { namespace Authorization
{
	Sid::Sid(PSID psid) :
		m_psid(psid)
	{
	}

	Sid::Sid(Sid&& other) :
		m_psid(nullptr)
	{
		MoveFrom(other);
	}

	Sid::~Sid()
	{
		Free();
	}

	Sid& Sid::operator=(Sid&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Void Sid::Free()
	{
		if (m_psid)
		{
			auto psid = m_psid;
			m_psid = nullptr;
			::FreeSid(psid);
		}
	}

	Void Sid::Attach(PSID psid)
	{
		Free();
		m_psid = psid;
	}

	PSID Sid::Detach()
	{
		auto psid = m_psid;
		m_psid = nullptr;
		return psid;
	}

	Bool Sid::IsAdmin()
	{
		SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
		Sid adminGroup;

		auto success = ::AllocateAndInitializeSid(
			&ntAuthority,
			2,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0,
			adminGroup.GetAddress());

		if (!success)
		{
			return False;
		}

		BOOL isMember = FALSE;

		success = ::CheckTokenMembership(NULL, adminGroup, &isMember);
		if (!success)
		{
			return False;
		}

		return TRUE == isMember;
	}

	Bool Sid::IsSystem()
	{
		SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
		Sid system;

		auto success = ::AllocateAndInitializeSid(
			&ntAuthority,
			1,
			SECURITY_LOCAL_SYSTEM_RID,
			0, 0, 0, 0, 0, 0, 0,
			system.GetAddress());

		if (!success)
		{
			return False;
		}

		BOOL isSystem = FALSE;

		success = ::CheckTokenMembership(NULL, system, &isSystem);
		if (!success)
		{
			return False;
		}

		return TRUE == isSystem;
	}

	Void Sid::MoveFrom(Sid& other)
	{
		if (this != &other)
		{
			Attach(other.Detach());
		}
	}

} } }