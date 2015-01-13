#pragma once
#include "..\..\Types.h"

namespace WinToolsLib { namespace Os { namespace Authorization
{
	class Impersonate
	{
	public:
		Impersonate();
		~Impersonate();

		static Void LoggedOnUser();
		static Void CreateProcessAsLoggedOnUser(
			const TChar* application,
			const TChar* parameters = nullptr);

	private:
		Bool m_revertToSelf;
	};

} } }