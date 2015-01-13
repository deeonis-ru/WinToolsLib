#include "stdafx.h"
#include <WinToolsLib\Buffer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(BufferTest)
	{
	public:
		TEST_METHOD(BufferToString)
		{
			Buffer source;
			String result = Buffer::ToString(source);

			Assert::AreEqual(True, result.IsEmpty());

			Byte code[] = { 0x0F, 0xAE, 0x01 };
			SizeT codeSize = sizeof(code);

			source = Buffer(code, codeSize);
			result = Buffer::ToString(source);

			Assert::AreEqual(6, (Int32)result.GetLength());
			
			Assert::AreEqual(Text("0FAE01"), result.GetBuffer());
		}

		TEST_METHOD(BufferFromString)
		{
			String source;
			Buffer result = Buffer::FromString(source);

			Assert::AreEqual(True, result.IsEmpty());

			source = String(Text("0FAE01"));
			result = Buffer::FromString(source);

			Assert::AreEqual(3, (Int32)result.GetSize());

			Assert::AreEqual(0x0F, (Int32)result.GetBuffer()[0]);
			Assert::AreEqual(0xAE, (Int32)result.GetBuffer()[1]);
			Assert::AreEqual(0x01, (Int32)result.GetBuffer()[2]);
		}
	};
}