#pragma once
#include "Types.h"
#include "String.h"

namespace WinToolsLib
{
	class System
	{
	public:
		static Bool IsXpOrLater();
		static Bool IsVistaOrLater();
		static Bool IsSevenOrLater();
		static Bool IsEightOrLater();
		static Void GetVersion(UInt32& major, UInt32& minor);
		static String GetFullVersion();
		static String GetSysDir();
		static String GetWinDir();
		static String GetSysWinDir();
		static String GetSysWowDir();
	};
}
