#include "stdafx.h"
#include <WinToolsLib\Checksum\Crc.h>
#include <WinToolsLib\Checksum\Fletcher.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;
using namespace WinToolsLib::Checksum;

namespace Tests
{
	TEST_CLASS(ChecksumTest)
	{
	public:
		TEST_METHOD(Crc32Calc)
		{
			[]()
			{
				auto input = "";
				auto hash = Crc32::Calc((PCByte)input, strlen(input));

				Assert::IsTrue(0 == hash);
			}();

			[]()
			{
				auto input = "The quick brown fox jumps over the lazy dog";
				auto hash = Crc32::Calc((PCByte)input, strlen(input));

				Assert::IsTrue(0x414fa339 == hash);
			}();

			[]()
			{
				auto input = L"The quick brown fox jumps over the lazy dog";
				auto hash = Crc32::Calc((PCByte)input, wcslen(input) * sizeof(WChar));

				Assert::IsTrue(0x38282c38 == hash);
			}();
		}

		TEST_METHOD(Fletcher16Calc)
		{
			[]()
			{
				auto input = "Wikipedia";
				auto hash = Fletcher16::Calc((PCByte)input, strlen(input));

				Assert::IsTrue(0xEE9A == hash);
			}();

			[]()
			{
				auto input = L"Wikipedia";
				auto hash = Fletcher16::Calc((PCByte)input, wcslen(input) * sizeof(WChar));

				Assert::IsTrue(0xDD9A == hash);
			}();
		}
	};
}