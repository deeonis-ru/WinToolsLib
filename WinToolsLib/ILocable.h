#pragma once
#include "Types.h"

namespace WinToolsLib
{
	class ILockable
	{
	public:
		virtual ~ILockable() {}

		virtual Void Lock() = 0;
		virtual Void Unlock() = 0;
	};
}
