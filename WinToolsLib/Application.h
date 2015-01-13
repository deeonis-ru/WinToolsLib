#pragma once
#include "Types.h"
#include "Buffer.h"
#include "String.h"

namespace WinToolsLib
{
	class Application
	{
	public:
		static Buffer LoadResource(const TChar* resName, const TChar* resType);
		static Void ExtractResource(const TChar* path, const TChar* resName, const TChar* resType);
		static String LoadStringResource(UInt32 id);
	};
}
