#include "stdafx.h"
#include <WinToolsLib\Net\Url.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(UrlTest)
	{
	public:
		TEST_METHOD(UrlEncode)
		{
			{
				auto input = Text("");
				auto output = Net::Url::Encode(input);

				Assert::AreEqual(Text(""), output);
			}
			{
				auto input = Text("text with spaces");
				auto output = Net::Url::Encode(input);

				Assert::AreEqual(Text("text%20with%20spaces"), output);
			}
			{
				auto input = Text("first\r\nsecond\r\n");
				auto output = Net::Url::Encode(input);

				Assert::AreEqual(Text("first%0D%0Asecond%0D%0A"), output);
			}
		}

		TEST_METHOD(UrlDecode)
		{
			{
				auto input = Text("");
				auto output = Net::Url::Decode(input);

				Assert::AreEqual(Text(""), output);
			}
			{
				auto input = Text("text%20with%20spaces");
				auto output = Net::Url::Decode(input);

				Assert::AreEqual(Text("text with spaces"), output);
			}
			{
				auto input = Text("first%0D%0Asecond%0D%0A");
				auto output = Net::Url::Decode(input);

				Assert::AreEqual(Text("first\r\nsecond\r\n"), output);
			}
		}
	};
}