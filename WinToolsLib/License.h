#pragma once
#include "Types.h"
#include "String.h"

namespace WinToolsLib
{
	class License
	{
	public:
		static String GetUserId();

	private:
		static String GetProcessorId();
		static String GetProductId();
	};
}
