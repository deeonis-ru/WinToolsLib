#include "TaskMem.h"

#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")

namespace WinToolsLib
{
	namespace Com
	{
		namespace Details
		{
			PVoid Alloc(SizeT bytes)
			{
				return ::CoTaskMemAlloc(bytes);
			}

			Void Free(PVoid p)
			{
				::CoTaskMemFree(p);
			}
		}
	}
}