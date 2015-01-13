#pragma once
#include "Types.h"

namespace WinToolsLib
{
	class Debug
	{
	public:
		Debug(UInt32 processId);
		~Debug();

		static Void KillProcessOnExit(Bool kill);

	private:
		UInt32 m_processId;
	};
}