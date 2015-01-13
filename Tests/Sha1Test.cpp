#include "stdafx.h"
#include <WinToolsLib\Cryptography\Sha.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(Sha1Test)
	{
	public:
		TEST_METHOD(Sha1ComputeHash)
		{
			[]()
			{
				Cryptography::Sha1 sha1;

				auto input = "";
				sha1.ComputeHash((PCByte)input, strlen(input));

				auto digest = sha1.GetDigest();
				auto output = Buffer::ToString(digest);

				Assert::AreEqual(Text("DA39A3EE5E6B4B0D3255BFEF95601890AFD80709"), output.GetBuffer());
			}();

			[]()
			{
				Cryptography::Sha1 sha1;
				
				auto input = "The quick brown fox jumps over the lazy dog";
				sha1.ComputeHash((PCByte)input, strlen(input));

				auto digest = sha1.GetDigest();
				auto output = Buffer::ToString(digest);

				Assert::AreEqual(Text("2FD4E1C67A2D28FCED849EE1BB76E7391B93EB12"), output.GetBuffer());
			}();

			[]()
			{
				Cryptography::Sha1 sha1;
				
				auto input = "The quick brown fox jumps over the lazy cog";
				sha1.ComputeHash((PCByte)input, strlen(input));

				auto digest = sha1.GetDigest();
				auto output = Buffer::ToString(digest);

				Assert::AreEqual(Text("DE9F2C7FD25E1B3AFAD3E85A0BD17D9B100DB4B3"), output.GetBuffer());
			}();

			[]() // Hash of StringA
			{
				Cryptography::Sha1 sha1;
				
				StringA input("The quick brown fox jumps over the lazy dog");
				sha1.ComputeHash(input);

				auto digest = sha1.GetDigest();
				auto output = Buffer::ToString(digest);

				Assert::AreEqual(Text("2FD4E1C67A2D28FCED849EE1BB76E7391B93EB12"), output.GetBuffer());
			}();

			[]() // Hash of StringW
			{
				Cryptography::Sha1 sha1;
				
				StringW input(L"The quick brown fox jumps over the lazy dog");
				sha1.ComputeHash(input);

				auto digest = sha1.GetDigest();
				auto output = Buffer::ToString(digest);

				Assert::AreEqual(Text("BD136CB58899C93173C33A90DDE95EAD0D0CF6DF"), output.GetBuffer());
			}();
		}
	};
}