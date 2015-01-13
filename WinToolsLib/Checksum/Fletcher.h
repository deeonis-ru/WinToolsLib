#pragma once
#include "..\Types.h"

namespace WinToolsLib { namespace Checksum
{
	class Fletcher16
	{
	public:
		static UInt16 Calc(PCByte data, SizeT size);
	};

} }