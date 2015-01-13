#include "ArgumentList.h"
#include "..\Exception.h"

#include <Windows.h>
#include <ShellAPI.h>
#pragma comment(lib, "Shell32.lib")

namespace WinToolsLib
{
	namespace CommandLine
	{
		ArgumentList::ArgumentList(const TChar* commandLine)
		{
			if (nullptr == commandLine)
			{
				return;
			}

			Int32 argc = 0;
			auto argv = ::CommandLineToArgvW(commandLine, &argc);
			if (nullptr == argv)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			for (auto i = 0; i < argc; i++)
			{
				m_arguments.push_back(String(argv[i]));
			}
		}

		Void ArgumentList::Append(const CommandList& list)
		{
			for (auto const& command : list)
			{
				String argument(Text("/"));
				argument.Append(command.GetName());

				m_arguments.push_back(std::move(argument));
				for (const auto& parameter : command.GetParameters())
				{
					if (parameter.Contains(Text(" ")) &&
						!parameter.BeginsWith(Text("\"")))
					{
						m_arguments.push_back(String::FormatS(Text("\"%s\""), parameter.GetBuffer()));
					}
					else
					{
						m_arguments.push_back(parameter);
					}
				}
			}
		}

		String ArgumentList::ToCommandLine() const
		{
			String commandLine;
			for (const auto& argument : m_arguments)
			{
				if (!commandLine.IsEmpty())
				{
					commandLine.Append(Text(" "));
				}

				if (argument.Contains(Text(" ")) &&
					!argument.BeginsWith(Text("\"")))
				{
					commandLine.Append(String::FormatS(Text("\"%s\""), argument.GetBuffer()));
				}
				else
				{
					commandLine.Append(argument);
				}
			}
			return commandLine;
		}

		CommandList ArgumentList::ToCommandList() const
		{
			CommandList list;
			Command* lastCommand = nullptr;
			for (const auto& argument : m_arguments)
			{
				if (argument.BeginsWith(Text("/")) ||
					argument.BeginsWith(Text("-")))
				{
					list.push_back(Command(argument.SubString(1)));
					lastCommand = &list.back();
				}
				else if (nullptr != lastCommand)
				{
					lastCommand->AddParameter(argument);
				}
			}
			return list;
		}
	}
}