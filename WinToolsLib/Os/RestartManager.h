#pragma once
#include "..\Types.h"
#include "..\Process\Process.h"

namespace WinToolsLib { namespace Os
{
	class RestartManager
	{
	public:
		RestartManager();
		~RestartManager();

		std::vector<WinToolsLib::Process::Process> GetResourceUsedProcessList(const TChar* path);

	private:
		DWORD m_session;
	};
} }