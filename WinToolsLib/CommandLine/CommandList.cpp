#include "CommandList.h"
#include "..\Exception.h"

namespace WinToolsLib
{
	namespace CommandLine
	{
		Void Command::MoveFrom(Command& other)
		{
			if (this != &other)
			{
				m_name = std::move(other.m_name);
				m_parameters = std::move(other.m_parameters);
			}
		}

		Void Command::CopyFrom(const Command& other)
		{
			if (this != &other)
			{
				m_name = other.m_name;
				m_parameters = other.m_parameters;
			}
		}
	}
}