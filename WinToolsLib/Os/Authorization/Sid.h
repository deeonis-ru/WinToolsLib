#pragma once
#include "..\..\Types.h"

#include <Windows.h>

namespace WinToolsLib { namespace Os { namespace Authorization
{
	class Sid
	{
		Sid(const Sid& other);            // Non-copyable
		Sid& operator=(const Sid& other); // Non-copyable

	public:
		Sid(PSID psid = nullptr);
		Sid(Sid&& other);
		~Sid();

		Sid& operator=(Sid&& other);

		Void Free();
		Void Attach(PSID psid);
		PSID Detach();
		
		operator PSID() const;
		PSID* GetAddress();

		static Bool IsAdmin();
		static Bool IsSystem();

	protected:
		Void MoveFrom(Sid& other);

	private:
		PSID m_psid;
	};

	inline Sid::operator PSID() const
	{
		return m_psid;
	}

	inline PSID* Sid::GetAddress()
	{
		return &m_psid;
	}

} } }