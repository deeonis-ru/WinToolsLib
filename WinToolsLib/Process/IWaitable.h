#pragma once
#include "..\Types.h"

namespace WinToolsLib { namespace Process
{
	class IWaitable
	{
	public:
		virtual ~IWaitable() {}

		virtual Void Wait() const = 0;
		virtual Void Wait(UInt32 milliseconds) const = 0;
	};
}}
