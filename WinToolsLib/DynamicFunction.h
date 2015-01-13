#pragma once
#include "Types.h"

namespace WinToolsLib
{
	template <class Fn>
	Fn DynamicFunction(const Char* module, const Char* function)
	{
		return (Fn)::GetProcAddress(::GetModuleHandleA(module), function);
	}
}
