#include "stdafx.h"
#include <WinToolsLib\CommandLine\ArgumentList.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(CommandLineTest)
	{
	public:
		TEST_METHOD(ArgumentList)
		{
			auto cmdLine = Text("\"C:\\Program Files\\Some Brand\\App.exe\" /command param \"second param\"");
			auto appPath = Text("C:\\Program Files\\Some Brand\\App.exe");
			auto appArgs = Text("/command param \"second param\"");
			auto command = Text("/command");
			auto param1 = Text("param");
			auto param2 = Text("second param");

			[=]()
			{
				CommandLine::ArgumentList argList(cmdLine);

				Assert::AreEqual((SizeT)4, argList.GetSize());
				Assert::AreEqual(cmdLine, argList.ToCommandLine());

				Assert::AreEqual(appPath, argList[0]);
				Assert::AreEqual(command, argList[1]);
				Assert::AreEqual(param1, argList[2]);
				Assert::AreEqual(param2, argList[3]);
			}();
			
			[=]()
			{
				CommandLine::ArgumentList argList;
				argList.PushBack(command);
				argList.PushBack(param1);
				argList.PushBack(param2);
				argList.PushFront(appPath);

				Assert::AreEqual((SizeT)4, argList.GetSize());

				Assert::AreEqual(cmdLine, argList.ToCommandLine());

				Assert::AreEqual(appPath, argList.PopFront());
				Assert::AreEqual(appArgs, argList.ToCommandLine());

				Assert::AreEqual(command, argList.PopFront());
				Assert::AreEqual(param1, argList.PopFront());
				Assert::AreEqual(param2, argList.PopFront());

				Assert::AreEqual((SizeT)0, argList.GetSize());
			}();
		}
		
		TEST_METHOD(CommandList)
		{
			auto cmdLine = Text("\"C:\\Program Files\\Some Brand\\App.exe\" /command param \"second param\" /another \"one more param\"");
			auto appPath = Text("C:\\Program Files\\Some Brand\\App.exe");
			auto cmdName1 = Text("command");
			auto param11 = Text("param");
			auto param12 = Text("second param");
			auto cmdName2 = Text("another");
			auto param21 = Text("one more param");

			[=]()
			{
				CommandLine::ArgumentList argList(cmdLine);

				auto cmdList = argList.ToCommandList();
				Assert::AreEqual((SizeT)2, cmdList.size());

				{
					auto command = std::move(cmdList.front());
					cmdList.pop_front();
					Assert::AreEqual(cmdName1, command.GetName());

					const auto& parameters = command.GetParameters();
					Assert::AreEqual((SizeT)2, parameters.size());

					Assert::AreEqual(param11, parameters[0]);
					Assert::AreEqual(param12, parameters[1]);
				}

				{
					auto command = std::move(cmdList.front());
					cmdList.pop_front();
					Assert::AreEqual(cmdName2, command.GetName());

					const auto& parameters = command.GetParameters();
					Assert::AreEqual((SizeT)1, parameters.size());

					Assert::AreEqual(param21, parameters[0]);
				}
			}();

			[=]()
			{
				CommandLine::CommandList cmdList;

				{
					CommandLine::Command command(cmdName1);
					command.AddParameter(param11);
					command.AddParameter(param12);

					cmdList.push_back(std::move(command));
				}

				{
					CommandLine::Command command(cmdName2);
					command.AddParameter(param21);

					cmdList.push_back(std::move(command));
				}

				CommandLine::ArgumentList argList;
				argList.PushBack(appPath);
				argList.Append(cmdList);

				Assert::AreEqual(cmdLine, argList.ToCommandLine());
			}();
		}
	};
}