#pragma once
#include "Types.h"

namespace WinToolsLib
{
	class Privilege
	{
	public:
		static Void Set(PTChar pszPrivilege, Bool enable);
	};

	class DebugPrivilege : protected Privilege
	{
	public:
		static Void Enable();
		static Void Disable();
	};
}
