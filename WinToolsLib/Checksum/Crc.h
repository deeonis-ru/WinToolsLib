#pragma once
#include "..\Types.h"

namespace WinToolsLib { namespace Checksum
{
	class Crc32
	{
	public:
		static UInt32 Calc(PCByte data, SizeT size);

	private:
		static UInt32 Update(UInt32 init, PCByte data, SizeT size);
	};

} }