#include "stdafx.h"
#include <WinToolsLib\BufferStream.h>
#include <WinToolsLib\Disasm.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(DisasmTest)
	{
	public:
		TEST_METHOD(DisasmJump)
		{
			Byte code[] = { 0xE9, 0x43, 0x59, 0x8C, 0x8B, 0xCC, 0xCC, 0xCC };
			SizeT codeSize = sizeof(code);

			BufferStream stream(Buffer(code, codeSize));

			Disasm disasm(&stream, 32);
			auto isHook = disasm.IsHook(0x7723aaa0, codeSize);

			Assert::AreEqual(True, isHook);

			auto targetVa = disasm.GetHookDestination();
			auto hookSize = disasm.GetHookSize();

			Assert::AreEqual(0x02b003e8ULL, targetVa);
			Assert::AreEqual(codeSize, hookSize);
		}

		TEST_METHOD(DisasmIndirectJump32)
		{
			Byte code[] = { 0x0A, 0x00, 0xAB, 0x71, 0x00, 0x00, 0xFF, 0x25, 0xFA, 0xFF, 0xAB, 0x71, 0xCC, 0xCC };
			SizeT codeSize = sizeof(code);
			SizeT codeOffset = 6;

			BufferStream stream(Buffer(code, codeSize));
			stream.SetPosition(codeOffset);

			Disasm disasm(&stream, 32);
			auto isHook = disasm.IsHook(0x71ac0000, codeSize);

			Assert::AreEqual(True, isHook);

			auto targetVa = disasm.GetHookDestination();
			auto hookSize = disasm.GetHookSize();

			Assert::AreEqual(0x71ab000aULL, targetVa);
			Assert::AreEqual(codeSize - codeOffset, hookSize);
		}

		TEST_METHOD(DisasmIndirectJump64)
		{
			Byte code[] = { 0xAC, 0x81, 0xCC, 0xEA, 0xFE, 0x07, 0x00, 0x00, 0xFF, 0x25, 0xF2, 0xFF, 0xFF, 0xFF, 0xCC, 0xCC };
			SizeT codeSize = sizeof(code);
			SizeT codeOffset = 8;

			BufferStream stream(Buffer(code, codeSize));
			stream.SetPosition(codeOffset);

			Disasm disasm(&stream, 64);
			auto isHook = disasm.IsHook(0x02b003e8, codeSize);

			Assert::AreEqual(True, isHook);

			auto targetVa = disasm.GetHookDestination();
			auto hookSize = disasm.GetHookSize();

			Assert::AreEqual(0x7feeacc81acULL, targetVa);
			Assert::AreEqual(codeSize - codeOffset, hookSize);
		}

		TEST_METHOD(DisasmPushRet)
		{
			Byte code[] = { 0x68, 0x78, 0x03, 0x5B, 0x02, 0xC3 };
			SizeT codeSize = sizeof(code);

			BufferStream stream(Buffer(code, codeSize));

			Disasm disasm(&stream, 64);
			auto isHook = disasm.IsHook(0x02b003e8, codeSize);

			Assert::AreEqual(True, isHook);

			auto targetVa = disasm.GetHookDestination();
			auto hookSize = disasm.GetHookSize();

			Assert::AreEqual(0x025b0378ULL, targetVa);
			Assert::AreEqual(codeSize, hookSize);
		}

		TEST_METHOD(DisasmMoveJump)
		{
			Byte code[] = { 0x48, 0xB8, 0x80, 0x69, 0x38, 0xCF, 0xFE, 0x07, 0x00, 0x00, 0xFF, 0xE0 };
			SizeT codeSize = sizeof(code);

			BufferStream stream(Buffer(code, codeSize));

			Disasm disasm(&stream, 64);
			auto isHook = disasm.IsHook(0x77129b80, codeSize);

			Assert::AreEqual(True, isHook);

			auto targetVa = disasm.GetHookDestination();
			auto hookSize = disasm.GetHookSize();

			Assert::AreEqual(0x7fecf386980ULL, targetVa);
			Assert::AreEqual(codeSize, hookSize);
		}

		TEST_METHOD(DisasmCallAbsoluteIndirect32)
		{
			Byte code[] = { 0xEF, 0xCD, 0xAB, 0x89, 0x00, 0x00, 0xFF, 0x15, 0x45, 0x8B, 0x1A, 0x75 };
			SizeT codeSize = sizeof(code);
			SizeT codeOffset = 6;

			BufferStream stream(Buffer(code, codeSize));

			Disasm disasm(&stream, 32);
			stream.SetPosition(codeOffset);
			
			auto isHook = disasm.IsHook(0x751a8b4b, codeSize - codeOffset);
			Assert::AreEqual(True, isHook);

			auto targetVa = disasm.GetHookDestination();
			Assert::AreEqual(0x89abcdefULL, targetVa);

			auto hookSize = disasm.GetHookSize();
			Assert::AreEqual(codeSize - codeOffset, hookSize);
		}

		TEST_METHOD(DisasmCallAbsoluteIndirect64)
		{
			Byte code[] = { 0xFF, 0x15, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0xCD, 0xFD, 0xFE, 0x07, 0x00, 0x00 };
			SizeT codeSize = sizeof(code);

			BufferStream stream(Buffer(code, codeSize));
			Disasm disasm(&stream, 64);
			
			auto isHook = disasm.IsHook(0x7fefdaf9aa3, codeSize);
			Assert::AreEqual(True, isHook);

			auto targetVa = disasm.GetHookDestination();
			Assert::AreEqual(0x7fefdcd000eULL, targetVa);

			auto hookSize = disasm.GetHookSize();
			Assert::AreEqual((SizeT)6, hookSize);
		}

		TEST_METHOD(DisasmCallRelative64)
		{
			Byte code[] = { 0xE8, 0x23, 0xC8, 0xD0, 0x8B };
			SizeT codeSize = sizeof(code);

			BufferStream stream(Buffer(code, codeSize));
			Disasm disasm(&stream, 64);
			
			auto isHook = disasm.IsHook(0x74ba34d0, codeSize);
			Assert::AreEqual(True, isHook);

			auto targetVa = disasm.GetHookDestination();
			Assert::AreEqual(0x8afcf8ULL, targetVa);

			auto hookSize = disasm.GetHookSize();
			Assert::AreEqual(codeSize, hookSize);
		}

		TEST_METHOD(DisasmTrampoline32)
		{
			Byte code[] =
			{
				0x57, 0x52, 0x51, 0x50, 0xBF, 0x3B, 0x00, 0xAB,  0x71, 0x8B, 0x54, 0x24, 0x10, 0xB9, 0x18, 0x01,
				0x00, 0x00, 0x33, 0xC0, 0xF0, 0x0F, 0xB1, 0x57,  0x01, 0x74, 0x09, 0x83, 0xC7, 0x0E, 0x33, 0xC0,
				0xE2, 0xF2, 0xEB, 0x04, 0x89, 0x7C, 0x24, 0x10,  0x58, 0x59, 0x5A, 0x5F, 0xE9, 0xF5, 0xA7, 0x57,
				0x9E, 0x68, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x83,  0x25, 0x3C, 0x00, 0xAB, 0x71, 0x00, 0xC3, 0x68,
			};
			SizeT codeSize = sizeof(code);

			BufferStream stream(Buffer(code, codeSize));
			Disasm disasm(&stream, 32);
			
			auto isHook = disasm.IsHook(0x71AB000A, codeSize);
			Assert::AreEqual(False, isHook);

			auto isTrampoline = disasm.IsTrampoline(0x71AB000A, codeSize);
			Assert::AreEqual(True, isTrampoline);

			auto targetVa = disasm.GetHookDestination();
			Assert::AreEqual(0x1002A830ULL, targetVa);
		}

		TEST_METHOD(DisasmTrampoline64)
		{
			Byte code[] =
			{
				0x57, 0x52, 0x51, 0x50, 0x48, 0x8D, 0x3D, 0x39,  0x00, 0x00, 0x00, 0x48, 0x8B, 0x54, 0x24, 0x20,
				0x48, 0xC7, 0xC1, 0xFA, 0x00, 0x00, 0x00, 0x48,  0x33, 0xC0, 0xF0, 0x48, 0x0F, 0xB1, 0x57, 0x10,
				0x74, 0x0B, 0x48, 0x83, 0xC7, 0x18, 0x48, 0x33,  0xC0, 0xE2, 0xEF, 0xEB, 0x05, 0x48, 0x89, 0x7C,
				0x24, 0x20, 0x58, 0x59, 0x5A, 0x5F, 0xFF, 0x25,  0x00, 0x00, 0x00, 0x00, 0x50, 0xE9, 0x00, 0x80,
				0x01, 0x00, 0x00, 0x00, 0xFF, 0x35, 0x0A, 0x00,  0x00, 0x00, 0xF0, 0x48, 0x83, 0x25, 0x01, 0x00,
				0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0xFF, 0x35, 0x0A, 0x00,
			};
			SizeT codeSize = sizeof(code);

			BufferStream stream(Buffer(code, codeSize));
			Disasm disasm(&stream, 64);
			
			auto isHook = disasm.IsHook(0x7FEFED6000E, codeSize);
			Assert::AreEqual(False, isHook);

			auto isTrampoline = disasm.IsTrampoline(0x7FEFED6000E, codeSize);
			Assert::AreEqual(True, isTrampoline);

			auto targetVa = disasm.GetHookDestination();
			Assert::AreEqual(0x18000E950ULL, targetVa);
		}
	};
}