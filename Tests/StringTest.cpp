#include "stdafx.h"
#include <WinToolsLib\String.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(StringTest)
	{
	public:
		TEST_METHOD(StringGetBuffer)
		{
			[]()
			{
				String empty;
				Assert::AreEqual(nullptr, empty.GetBuffer());
				Assert::AreEqual(L"", (const TChar*)empty);
			}();

			[]()
			{
				const String empty;
				Assert::AreEqual(nullptr, empty.GetBuffer());
				Assert::AreEqual(L"", (const TChar*)empty);
			}();
		}
		
		TEST_METHOD(StringGetLength)
		{
			Assert::AreEqual((SizeT)0, StringA::GetLength(nullptr));
			Assert::AreEqual((SizeT)0, StringW::GetLength(nullptr));
			
			Assert::AreEqual((SizeT)0, StringA::GetLength(""));
			Assert::AreEqual((SizeT)0, StringW::GetLength(L""));
			
			Assert::AreEqual((SizeT)1, StringA::GetLength(" "));
			Assert::AreEqual((SizeT)1, StringW::GetLength(L" "));
		}

		TEST_METHOD(StringMatch)
		{
			[]()
			{
				StringA string("SomeMask_1234.zzz");

				Assert::AreEqual(True, string.Match("SomeMask_1234.zzz"));
				Assert::AreEqual(True, string.Match("somemask_1234.zzz"));
				Assert::AreEqual(True, string.Match("SOMEMASK_1234.ZZZ"));
				Assert::AreEqual(True, string.Match("sOMEmASK_1234.ZzZ"));
				Assert::AreEqual(True, string.Match("SomeMask_????.zzz"));
				Assert::AreEqual(True, string.Match("SomeMask_1234.*"));
				Assert::AreEqual(True, string.Match("SomeMask_????.*"));
				Assert::AreEqual(True, string.Match("*.*"));
				Assert::AreEqual(True, string.Match("*"));
			
				Assert::AreEqual(False, string.Match("SomeMask_??.zzz"));
				Assert::AreEqual(False, string.Match("SomeMask_????."));
				Assert::AreEqual(False, string.Match("SomeMask_*."));
				Assert::AreEqual(False, string.Match("SomeMask_00*.zzz"));
				Assert::AreEqual(False, string.Match("SomeMask_*.z"));
				Assert::AreEqual(False, string.Match("Other"));
			}();

			[]()
			{
				StringW string(L"SomeMask_1234.zzz");

				Assert::AreEqual(True, string.Match(L"SomeMask_1234.zzz"));
				Assert::AreEqual(True, string.Match(L"somemask_1234.zzz"));
				Assert::AreEqual(True, string.Match(L"SOMEMASK_1234.ZZZ"));
				Assert::AreEqual(True, string.Match(L"sOMEmASK_1234.ZzZ"));
				Assert::AreEqual(True, string.Match(L"SomeMask_????.zzz"));
				Assert::AreEqual(True, string.Match(L"SomeMask_1234.*"));
				Assert::AreEqual(True, string.Match(L"SomeMask_????.*"));
				Assert::AreEqual(True, string.Match(L"*.*"));
				Assert::AreEqual(True, string.Match(L"*"));
			
				Assert::AreEqual(False, string.Match(L"SomeMask_??.zzz"));
				Assert::AreEqual(False, string.Match(L"SomeMask_????."));
				Assert::AreEqual(False, string.Match(L"SomeMask_*."));
				Assert::AreEqual(False, string.Match(L"SomeMask_00*.zzz"));
				Assert::AreEqual(False, string.Match(L"SomeMask_*.z"));
				Assert::AreEqual(False, string.Match(L"Other"));
			}();
		}
		
		TEST_METHOD(StringReplace)
		{
			[]()
			{
				StringA string("Some text with marker (not mark)!");
				string.Replace("marker", "value");
				Assert::AreEqual("Some text with value (not mark)!", string);
			}();

			[]()
			{
				StringW string(L"Some text with marker (not mark)!");
				string.Replace(L"marker", L"value");
				Assert::AreEqual(L"Some text with value (not mark)!", string);
			}();

			[]()
			{
				StringA string("* some * text *");
				string.Replace("*", "123");
				Assert::AreEqual("123 some 123 text 123", string);
			}();

			[]()
			{
				StringW string(L"* some * text *");
				string.Replace(L"*", L"123");
				Assert::AreEqual(L"123 some 123 text 123", string);
			}();

			[]() // Infinite loop test
			{
				String string = Text("C:\\Windows\\regedit.exe");
				string.Replace(Text("C:\\Windows"), Text("C:\\Windows\\SysWOW64"));
				Assert::AreEqual(Text("C:\\Windows\\SysWOW64\\regedit.exe"), string);
			}();
		}
		
		TEST_METHOD(StringFormatS)
		{
			[]()
			{
				auto string = StringA::FormatS("%d %u 0x%X %s %S", -1, 2147483648, 0xA000, "ascii", L"wide");
				Assert::AreEqual("-1 2147483648 0xA000 ascii wide", string);
			}();

			[]()
			{
				auto string = StringB::FormatS((PCByte)"%d %u 0x%X %s %S", -1, 2147483648, 0xA000, "ascii", L"wide");
				Assert::AreEqual("-1 2147483648 0xA000 ascii wide", (PCChar)(PCByte)string);
			}();

			[]()
			{
				auto string = StringW::FormatS(L"%d %u 0x%X %s %S", -1, 2147483648, 0xA000, L"wide", "ascii");
				Assert::AreEqual(L"-1 2147483648 0xA000 wide ascii", string);
			}();
		}
		
		TEST_METHOD(StringConvert)
		{
			[]() // ASCII -> UTF8
			{
				StringA string("text текст");
				Assert::AreEqual("text \xD1\x82\xD0\xB5\xD0\xBA\xD1\x81\xD1\x82", (PCChar)(PCByte)StringB::Convert(string));
			}();

			[]() // ASCII -> UTF16
			{
				StringA string("text текст");
				Assert::AreEqual(L"text текст", StringW::Convert(string));
			}();

			[]() // UTF8 -> ASCII
			{
				StringB string((PCByte)"text \xD1\x82\xD0\xB5\xD0\xBA\xD1\x81\xD1\x82");
				Assert::AreEqual("text текст", StringA::Convert(string));
			}();

			[]() // UTF8 -> UTF16
			{
				StringB string((PCByte)"text \xD1\x82\xD0\xB5\xD0\xBA\xD1\x81\xD1\x82");
				Assert::AreEqual(L"text текст", StringW::Convert(string));
			}();
						
			[]() // UTF16 -> ASCII
			{
				StringW string(L"text текст");
				Assert::AreEqual("text текст", StringA::Convert(string));
			}();
			
			[]() // UTF16 -> UTF8
			{
				StringW string(L"text текст");
				Assert::AreEqual("text \xD1\x82\xD0\xB5\xD0\xBA\xD1\x81\xD1\x82", (PCChar)(PCByte)StringB::Convert(string));
			}();

			[]() // ASCII
			{
				Assert::AreEqual("-128",                 StringA::Convert((Int8)(128)));
				Assert::AreEqual("-32768",               StringA::Convert((Int16)(32768)));
				Assert::AreEqual("-2147483648",          StringA::Convert((Int32)(2147483648)));
				Assert::AreEqual("-9223372036854775808", StringA::Convert((Int64)(9223372036854775808ULL)));

				Assert::AreEqual("128",                 StringA::Convert((UInt8)(128)));
				Assert::AreEqual("32768",               StringA::Convert((UInt16)(32768)));
				Assert::AreEqual("2147483648",          StringA::Convert((UInt32)(2147483648)));
				Assert::AreEqual("9223372036854775808", StringA::Convert((UInt64)(9223372036854775808ULL)));
			}();

			[]() // UTF8
			{
				Assert::AreEqual("-128",                 (PCChar)(PCByte)StringB::Convert((Int8)(128)));
				Assert::AreEqual("-32768",               (PCChar)(PCByte)StringB::Convert((Int16)(32768)));
				Assert::AreEqual("-2147483648",          (PCChar)(PCByte)StringB::Convert((Int32)(2147483648)));
				Assert::AreEqual("-9223372036854775808", (PCChar)(PCByte)StringB::Convert((Int64)(9223372036854775808ULL)));

				Assert::AreEqual("128",                 (PCChar)(PCByte)StringB::Convert((UInt8)(128)));
				Assert::AreEqual("32768",               (PCChar)(PCByte)StringB::Convert((UInt16)(32768)));
				Assert::AreEqual("2147483648",          (PCChar)(PCByte)StringB::Convert((UInt32)(2147483648)));
				Assert::AreEqual("9223372036854775808", (PCChar)(PCByte)StringB::Convert((UInt64)(9223372036854775808ULL)));
			}();

			[]() // UTF16
			{
				Assert::AreEqual(L"-128",                 StringW::Convert((Int8)(128)));
				Assert::AreEqual(L"-32768",               StringW::Convert((Int16)(32768)));
				Assert::AreEqual(L"-2147483648",          StringW::Convert((Int32)(2147483648)));
				Assert::AreEqual(L"-9223372036854775808", StringW::Convert((Int64)(9223372036854775808ULL)));

				Assert::AreEqual(L"128",                 StringW::Convert((UInt8)(128)));
				Assert::AreEqual(L"32768",               StringW::Convert((UInt16)(32768)));
				Assert::AreEqual(L"2147483648",          StringW::Convert((UInt32)(2147483648)));
				Assert::AreEqual(L"9223372036854775808", StringW::Convert((UInt64)(9223372036854775808ULL)));
			}();
		}
		
		TEST_METHOD(StringSplit)
		{
			[]()
			{
				StringA string("A|B||D");
				StringA by("|");
				StringA token;
				
				SizeT start = 0;
				SizeT end = -1;

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("A", token);

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("B", token);

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::IsTrue(token.IsEmpty());

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("D", token);

				token = "E";
				Assert::AreEqual(end, string.Split(by, token, start));
				Assert::AreEqual("E", token);
			}();

			[]()
			{
				StringA string("line 1\r\nsecond line\r\nl3\r\n");
				StringA by("\r\n");
				StringA token;
				
				SizeT start = 0;
				SizeT end = -1;

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("line 1", token);

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("second line", token);

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("l3", token);

				Assert::AreEqual(end, string.Split(by, token, start));
			}();

			[]()
			{
				StringA string("A|B||D");
				StringA by("|");
				std::vector<StringA> tokenList;

				string.Split(by, tokenList);

				Assert::IsTrue(3 == tokenList.size());
				Assert::AreEqual("A", tokenList[0]);
				Assert::AreEqual("B", tokenList[1]);
				Assert::AreEqual("D", tokenList[2]);
			}();

			[]()
			{
				StringA string("A|B||D");
				StringA by(",");
				std::vector<StringA> tokenList;

				string.Split(by, tokenList);

				Assert::IsTrue(1 == tokenList.size());
				Assert::AreEqual("A|B||D", tokenList[0]);
			}();

			[]()
			{
				StringA string("A|B||D");
				StringA by("");
				StringA token;

				SizeT start = 0;
				SizeT end = -1;

				Assert::AreEqual(string.GetLength(), string.Split(by, token, start));
				Assert::AreEqual(end, string.Split(by, token, start));
				Assert::AreEqual("A|B||D", token);
			}();
		}
		
		TEST_METHOD(StringIsEqual)
		{
			[]()
			{
				const TChar* ptr = Text("value");
				String string(ptr);

				Assert::IsTrue(string.IsEqual(ptr));
				Assert::IsTrue(string == ptr);
				Assert::IsTrue(ptr == string);
			}();

			[]()
			{
				const TChar* ptr = Text("value");
				String string(Text("value"));

				Assert::IsTrue(string.IsEqual(ptr));
				Assert::IsTrue(string == ptr);
				Assert::IsTrue(ptr == string);
			}();

			[]()
			{
				const TChar* ptr = Text("value1");
				String string(Text("value2"));

				Assert::IsFalse(string.IsEqual(ptr));
				Assert::IsFalse(string == ptr);
				Assert::IsFalse(ptr == string);
			}();

			[]()
			{
				const TChar* ptr = nullptr;
				String string(Text("value"));

				Assert::IsFalse(string.IsEqual(ptr));
				Assert::IsFalse(string == ptr);
				Assert::IsFalse(ptr == string);
			}();

			[]()
			{
				const TChar* ptr = Text("value");
				String string;

				Assert::IsFalse(string.IsEqual(ptr));
				Assert::IsFalse(string == ptr);
				Assert::IsFalse(ptr == string);
			}();

			[]()
			{
				const TChar* ptr = nullptr;
				String string;

				Assert::IsTrue(string.IsEqual(ptr));
				Assert::IsTrue(string == ptr);
				Assert::IsTrue(ptr == string);
			}();

			[]()
			{
				String string1;
				String string2;

				Assert::IsTrue(string1.IsEqual(string2));
				Assert::IsTrue(string1 == string2);
			}();

			[]()
			{
				String string1(Text("value1"));
				String string2(Text("value2"));

				Assert::IsFalse(string1.IsEqual(string2));
				Assert::IsFalse(string1 == string2);
			}();
		}
		
		TEST_METHOD(StringEndsWith)
		{
			[]()
			{
				const TChar* what = Text("");
				String string(Text(""));

				Assert::IsTrue(string.EndsWith(what));
			}();

			[]()
			{
				const TChar* what = Text("");
				String string(Text("Some"));

				Assert::IsTrue(string.EndsWith(what));
			}();

			[]()
			{
				const TChar* what = Text("Some");
				String string(Text("Som"));

				Assert::IsFalse(string.EndsWith(what));
			}();

			[]()
			{
				const TChar* what = Text("etc...");
				String string(Text("First, second, etc..."));

				Assert::IsTrue(string.EndsWith(what));
			}();

			[]()
			{
				const TChar* what = Text("etc");
				String string(Text("First, second, etc..."));

				Assert::IsFalse(string.EndsWith(what));
			}();
		}
		
		TEST_METHOD(StringToLower)
		{
			[]()
			{
				String string(Text(""));
				string.ToLower();
				Assert::AreEqual(nullptr, string.GetBuffer());
			}();

			[]()
			{
				String string(Text("abcdefghijklmnopqrstuvwxyz"));
				string.ToLower();
				Assert::AreEqual(Text("abcdefghijklmnopqrstuvwxyz"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
				string.ToLower();
				Assert::AreEqual(Text("abcdefghijklmnopqrstuvwxyz"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("Some0123"));
				string.ToLower();
				Assert::AreEqual(Text("some0123"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("100 KB"));
				string.ToLower();
				Assert::AreEqual(Text("100 kb"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("Мои Документы"));
				string.ToLower();
				Assert::AreEqual(Text("мои документы"), string.GetBuffer());
			}();
		}
		
		TEST_METHOD(StringToUpper)
		{
			[]()
			{
				String string(Text(""));
				string.ToUpper();
				Assert::AreEqual(nullptr, string.GetBuffer());
			}();

			[]()
			{
				String string(Text("abcdefghijklmnopqrstuvwxyz"));
				string.ToUpper();
				Assert::AreEqual(Text("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("abcdefghijklmnopqrstuvwxyz"));
				string.ToUpper();
				Assert::AreEqual(Text("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("some 123"));
				string.ToUpper();
				Assert::AreEqual(Text("SOME 123"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("100kb"));
				string.ToUpper();
				Assert::AreEqual(Text("100KB"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("Мои Документы"));
				string.ToUpper();
				Assert::AreEqual(Text("МОИ ДОКУМЕНТЫ"), string.GetBuffer());
			}();
		}
		
		TEST_METHOD(StringTrim)
		{
			[]()
			{
				String string(Text("Foo bar"));
				string.Trim(nullptr);
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("Foo bar"));
				string.Trim(Text(""));
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text(" Foo bar "));
				string.Trim(Text(""));
				Assert::AreEqual(Text(" Foo bar "), string.GetBuffer());
			}();

			[]()
			{
				String string(Text(" Foo bar "));
				string.Trim(Text(" "));
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("       Foo bar   "));
				string.Trim(Text("\t "));
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("\tFoo bar\t"));
				string.Trim(Text("\t"));
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("\t \t Foo bar \t \t"));
				string.Trim(Text(" \t"));
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();
		}
		
		TEST_METHOD(StringTrimLeft)
		{
			[]()
			{
				String string(Text("Foo bar"));
				string.TrimLeft(nullptr);
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("Foo bar"));
				string.TrimLeft(Text(""));
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text(" Foo bar "));
				string.TrimLeft(Text(""));
				Assert::AreEqual(Text(" Foo bar "), string.GetBuffer());
			}();

			[]()
			{
				String string(Text(" Foo bar "));
				string.TrimLeft(Text(" "));
				Assert::AreEqual(Text("Foo bar "), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("       Foo bar   "));
				string.TrimLeft(Text("\t "));
				Assert::AreEqual(Text("Foo bar   "), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("\tFoo bar\t"));
				string.TrimLeft(Text("\t"));
				Assert::AreEqual(Text("Foo bar\t"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("\t \t Foo bar \t \t"));
				string.TrimLeft(Text(" \t"));
				Assert::AreEqual(Text("Foo bar \t \t"), string.GetBuffer());
			}();
		}
		
		TEST_METHOD(StringTrimRight)
		{
			[]()
			{
				String string(Text("Foo bar"));
				string.TrimRight(nullptr);
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("Foo bar"));
				string.TrimRight(Text(""));
				Assert::AreEqual(Text("Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text(" Foo bar "));
				string.TrimRight(Text(""));
				Assert::AreEqual(Text(" Foo bar "), string.GetBuffer());
			}();

			[]()
			{
				String string(Text(" Foo bar "));
				string.TrimRight(Text(" "));
				Assert::AreEqual(Text(" Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("       Foo bar   "));
				string.TrimRight(Text("\t "));
				Assert::AreEqual(Text("       Foo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("\tFoo bar\t"));
				string.TrimRight(Text("\t"));
				Assert::AreEqual(Text("\tFoo bar"), string.GetBuffer());
			}();

			[]()
			{
				String string(Text("\t \t Foo bar \t \t"));
				string.TrimRight(Text(" \t"));
				Assert::AreEqual(Text("\t \t Foo bar"), string.GetBuffer());
			}();
		}
	};
}