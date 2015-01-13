#pragma once
#include "..\..\Types.h"
#include "..\..\Handles\Handle.h"

namespace WinToolsLib { namespace Os { namespace RemoteDesktop
{
	class Session
	{
	public:
		static Handles::Handle GetActiveUserToken();
	};

} } }