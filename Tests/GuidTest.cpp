#include "stdafx.h"
#include <WinToolsLib\Guid.h>

#include <algorithm>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(GuidTest)
	{
		Void AreEqual(const GUID& left, const GUID& right)
		{
			if (left.Data1 != right.Data1 ||
				left.Data2 != right.Data2 ||
				left.Data3 != right.Data3 ||
				left.Data4[0] != right.Data4[0] ||
				left.Data4[1] != right.Data4[1] ||
				left.Data4[2] != right.Data4[2] ||
				left.Data4[3] != right.Data4[3] ||
				left.Data4[4] != right.Data4[4] ||
				left.Data4[5] != right.Data4[5] ||
				left.Data4[6] != right.Data4[6] ||
				left.Data4[7] != right.Data4[7])
			{
				Assert::Fail(L"GUIDs are not equal");
			}
		}

	public:
		TEST_METHOD(GuidFromString)
		{
			const GUID expected = 
			{ 0xb166fb56, 0x7be7, 0x4c02, { 0xb0, 0x15, 0x91, 0x3, 0xa3, 0xa2, 0x2, 0x66 } };

			Guid actual(Text("B166FB56-7BE7-4C02-B015-9103A3A20266"));
			AreEqual(expected, actual);
		}

		TEST_METHOD(GuidToString)
		{
			static const GUID source = 
			{ 0x7307f638, 0x5f58, 0x436c, { 0xbd, 0x8b, 0x9, 0x76, 0xd3, 0xd0, 0xa3, 0x7e } };

			const auto expected = Text("7307F638-5F58-436C-BD8B-0976D3D0A37E");

			Guid actual(source);
			Assert::AreEqual(expected, actual.ToString());
		}

		TEST_METHOD(GuidEqual)
		{
			// {7F557ED4-E8C7-4049-BB99-8F1B4B898844}
			const GUID guid1 = 
			{ 0x7f557ed4, 0xe8c7, 0x4049, { 0xbb, 0x99, 0x8f, 0x1b, 0x4b, 0x89, 0x88, 0x44 } };
			Guid guidObj1(guid1);
			Guid guidObj2(guid1);

			Assert::IsTrue(guidObj1 == guidObj2);

			// {80E178E1-6C0A-4A22-8BEB-6896B15CC666}
			const GUID guid2 = 
			{ 0x80e178e1, 0x6c0a, 0x4a22, { 0x8b, 0xeb, 0x68, 0x96, 0xb1, 0x5c, 0xc6, 0x66 } };
			Guid guidObj3(guid2);
			Assert::IsFalse(guidObj1 == guidObj3);
		}
		
		TEST_METHOD(GuidNotEqual)
		{
			// {7F557ED4-E8C7-4049-BB99-8F1B4B898844}
			const GUID guid1 = 
			{ 0x7f557ed4, 0xe8c7, 0x4049, { 0xbb, 0x99, 0x8f, 0x1b, 0x4b, 0x89, 0x88, 0x44 } };
			Guid guidObj1(guid1);

			// {80E178E1-6C0A-4A22-8BEB-6896B15CC666}
			const GUID guid2 = 
			{ 0x80e178e1, 0x6c0a, 0x4a22, { 0x8b, 0xeb, 0x68, 0x96, 0xb1, 0x5c, 0xc6, 0x66 } };
			Guid guidObj2(guid2);

			Assert::IsTrue(guidObj1 != guidObj2);

			Guid guidObj3(guid1);
			Assert::IsFalse(guidObj1 != guidObj3);
		}

		TEST_METHOD(GuidCopy)
		{
			{
				Guid guid(Text("B8D27B78-C3C2-4149-9273-682A7B325D37"));
				Guid copy;

				Assert::IsFalse(guid.IsNull());
				Assert::IsTrue(copy.IsNull());

				copy = guid;

				Assert::IsFalse(guid.IsNull());
				Assert::IsFalse(copy.IsNull());

				AreEqual(copy, guid);
			}
			{
				Guid guid(Text("FE80AF2C-BE87-46DD-96C0-22C334FDEA3D"));
				Guid copy(guid);

				Assert::IsFalse(guid.IsNull());
				Assert::IsFalse(copy.IsNull());

				AreEqual(copy, guid);
			}
		}

		TEST_METHOD(GuidMove)
		{
			{
				Guid guid(Text("933F8CE8-0507-4A8E-94EA-672FEA924967"));
				Guid null;

				Assert::IsFalse(guid.IsNull());
				Assert::IsTrue(null.IsNull());

				null = std::move(guid);

				Assert::IsTrue(guid.IsNull());
				Assert::IsFalse(null.IsNull());
			}
			{
				Guid guid(Text("BFD5BEF6-272D-45BD-9F61-08DF1E13F52A"));
				Guid null(std::move(guid));

				Assert::IsTrue(guid.IsNull());
				Assert::IsFalse(null.IsNull());
			}
		}

		TEST_METHOD(GuidCompare)
		{
			{
				std::vector<Guid> guids;
				guids.push_back(Guid(Text("FACFCB9D-3219-4B1F-9EFE-656CF934D2C2")));
				guids.push_back(Guid(Text("182A463F-AD64-4879-B842-29154B6A1093")));
				guids.push_back(Guid(Text("71B382D5-5FE4-4241-8FA1-0CDE2D87F8DE")));
				guids.push_back(Guid(Text("179F6210-839E-4C58-8892-D6DA973ABD97")));

				std::sort(std::begin(guids), std::end(guids));

				Assert::IsTrue(guids[0] == Guid(Text("71B382D5-5FE4-4241-8FA1-0CDE2D87F8DE")));
				Assert::IsTrue(guids[1] == Guid(Text("FACFCB9D-3219-4B1F-9EFE-656CF934D2C2")));
				Assert::IsTrue(guids[2] == Guid(Text("182A463F-AD64-4879-B842-29154B6A1093")));
				Assert::IsTrue(guids[3] == Guid(Text("179F6210-839E-4C58-8892-D6DA973ABD97")));
			}
		}
	};
}