#pragma once
#include "Types.h"
#include "IStream.h"

namespace WinToolsLib
{
	class IParsable
	{
	public:
		virtual ~IParsable() {}

		virtual Void Parse(IStream& stream) = 0;
	};
}
