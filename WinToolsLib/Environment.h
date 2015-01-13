#pragma once
#include "Types.h"
#include "String.h"
#include "Handles\Handle.h"

namespace WinToolsLib
{
	using namespace Handles;

	class Environment
	{
	public:
		static String ExpandString(const TChar* pszString, const Handle& hToken = Handle());
		static String GetVariable(const TChar* name);
	};
}
