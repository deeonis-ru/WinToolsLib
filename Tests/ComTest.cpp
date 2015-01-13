#include "stdafx.h"
#include <WinToolsLib\Com\BStr.h>
#include <WinToolsLib\Com\SafeArray.h>
#include <WinToolsLib\Com\Variant.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(ComTest)
	{
	public:
		TEST_METHOD(ComBStr)
		{
			[]() // Empty
			{
				Com::BStr bstr;
				
				Assert::IsTrue(0 == bstr.GetLength());
				Assert::IsNull(bstr.operator BSTR());
			}();

			[]() // Empty string
			{
				Com::BStr bstr(L"");
				
				Assert::IsTrue(0 == bstr.GetLength());
				Assert::IsNotNull(bstr.operator BSTR());

				bstr.Free();
				
				Assert::IsTrue(0 == bstr.GetLength());
				Assert::IsNull(bstr.operator BSTR());
			}();

			[]() // Not empty string
			{
				const auto value = L"Test string!";
				Com::BStr bstr(value);
				
				Assert::IsTrue(12 == bstr.GetLength());
				Assert::IsTrue(value != bstr.operator BSTR());
				Assert::AreEqual(value, bstr.operator BSTR());
			}();

			[]() // BStr::Alloc and BStr::Free
			{
				const OLECHAR value[] = L"Test!";
				const auto length = (UInt32)(sizeof(value)/sizeof(value[0])) - 1;

				auto bstr = Com::BStr::Alloc(length);
				::CopyMemory(bstr.GetBuffer(), value, (length + 1)*sizeof(value[0]));

				Assert::IsTrue(length == bstr.GetLength());
				Assert::AreEqual(value, bstr.GetBuffer());

				bstr.Free();

				Assert::IsTrue(0 == bstr.GetLength());
				Assert::IsTrue(nullptr == bstr.GetBuffer());
			}();

			[]() // BStr::Attach
			{
				Com::BStr bstr;
				bstr.Attach(::SysAllocString(L"My string"));
				
				Assert::IsTrue(9 == bstr.GetLength());
				Assert::IsNotNull(bstr.operator BSTR());
			}();

			[]() // BStr::Detach
			{
				Com::BStr bstr(L"My string");
				auto value = bstr.Detach();
				
				Assert::IsTrue(0 == bstr.GetLength());
				Assert::IsNull(bstr.operator BSTR());

				::SysFreeString(value);
			}();

			[]() // Copy constructor
			{
				Com::BStr first(L"First string");
				Com::BStr second = first;

				Assert::IsTrue(12 == first.GetLength());
				Assert::IsTrue(12 == second.GetLength());

				Assert::IsTrue(first.operator BSTR() != second.operator BSTR());
				Assert::AreEqual(L"First string", first.operator BSTR());
				Assert::AreEqual(L"First string", second.operator BSTR());
			}();

			[]() // Copy assignment operator
			{
				Com::BStr first(L"First string");
				Com::BStr second(L"Second string");

				second = first;

				Assert::IsTrue(12 == first.GetLength());
				Assert::IsTrue(12 == second.GetLength());

				Assert::IsTrue(first.operator BSTR() != second.operator BSTR());
				Assert::AreEqual(L"First string", first.operator BSTR());
				Assert::AreEqual(L"First string", second.operator BSTR());
			}();

			[]() // Move constructor
			{
				Com::BStr first(L"First string");
				Com::BStr second = std::move(first);

				Assert::IsTrue(0 == first.GetLength());
				Assert::IsTrue(12 == second.GetLength());

				Assert::IsTrue(nullptr == first.operator BSTR());
				Assert::AreEqual(L"First string", second.operator BSTR());
			}();

			[]() // Move assignment operator
			{
				Com::BStr first(L"First string");
				Com::BStr second(L"Second string");

				second = std::move(first);

				Assert::IsTrue(0 == first.GetLength());
				Assert::IsTrue(12 == second.GetLength());

				Assert::IsTrue(nullptr == first.operator BSTR());
				Assert::AreEqual(L"First string", second.operator BSTR());
			}();
		}
		
		TEST_METHOD(ComSafeArray)
		{
			[]() // Create/Destroy
			{
				const auto size = 10U;
				auto safeArray = Com::SafeArray::Create(VT_UI4, size);

				Assert::IsTrue(size == safeArray.GetSize());

				safeArray.Destroy();

				Assert::IsTrue(0 == safeArray.GetSize());
			}();

			[]() // Attach/Detach
			{
				auto size = 10U;
				auto safeArray = Com::SafeArray::Create(VT_UI4, size);

				Assert::IsTrue(size == safeArray.GetSize());

				size = 5;
				safeArray.Attach(::SafeArrayCreateVector(VT_I8, 0, size));

				Assert::IsTrue(size == safeArray.GetSize());

				::SafeArrayDestroy(safeArray.Detach());

				Assert::IsTrue(0 == safeArray.GetSize());
			}();

			[]() // Copy constructor
			{
				auto first = Com::SafeArray::Create(VT_I1, 3);
				auto second = first;

				Assert::IsTrue(3 == first.GetSize());
				Assert::IsTrue(3 == second.GetSize());
			}();

			[]() // Copy assignment operator
			{
				auto first = Com::SafeArray::Create(VT_I1, 3);
				auto second = Com::SafeArray::Create(VT_I8, 7);

				second = first;

				Assert::IsTrue(3 == first.GetSize());
				Assert::IsTrue(3 == second.GetSize());
			}();

			[]() // Move constructor
			{
				auto first = Com::SafeArray::Create(VT_I1, 3);
				auto second = std::move(first);

				Assert::IsTrue(0 == first.GetSize());
				Assert::IsTrue(3 == second.GetSize());
			}();

			[]() // Move assignment operator
			{
				auto first = Com::SafeArray::Create(VT_I1, 3);
				auto second = Com::SafeArray::Create(VT_I8, 7);

				second = std::move(first);

				Assert::IsTrue(0 == first.GetSize());
				Assert::IsTrue(3 == second.GetSize());
			}();
		}
		
		TEST_METHOD(ComVariant)
		{
			[]() // FromStringArray, ToStringArray
			{
				Com::Variant::StringArray stringArray;
				stringArray.push_back(L"First");
				stringArray.push_back(L"Second");
				stringArray.push_back(L"Third");

				auto variant = Com::Variant::FromStringArray(stringArray);
				Com::SafeArray safeArray(variant->parray);

				Assert::IsTrue((VT_ARRAY | VT_BSTR) == variant->vt);
				Assert::IsTrue(3 == safeArray.GetSize());

				safeArray.Detach();
				stringArray.clear();
				stringArray = variant.ToStringArray();

				Assert::IsTrue(3 == stringArray.size());
				Assert::AreEqual(L"First", stringArray[0]);
				Assert::AreEqual(L"Second", stringArray[1]);
				Assert::AreEqual(L"Third", stringArray[2]);
			}();

			[]() // ToStringArray
			{
				VARIANT variant;
				::VariantInit(&variant);

				variant.vt = VT_UI4;
				variant.ulVal = 0x12345;

				auto wrong = Com::Variant(&variant);

				Assert::ExpectException<Com::VariantWrongTypeException>([&wrong]()
				{
					auto stringArray = wrong.ToStringArray();
				});

				auto empty = Com::Variant();
				auto stringArray = empty.ToStringArray();

				Assert::IsTrue(0 == stringArray.size());
			}();

			[]() // Copy constructor
			{
				VARIANT variant;
				::VariantInit(&variant);

				variant.vt = VT_BSTR;
				variant.bstrVal = ::SysAllocString(L"Some string");

				auto first = Com::Variant(&variant);
				auto second = first;

				Assert::IsTrue(VT_BSTR == first->vt);
				Assert::IsTrue(VT_BSTR == second->vt);

				Assert::AreEqual(L"Some string", first->bstrVal);
				Assert::AreEqual(L"Some string", second->bstrVal);

				Assert::AreNotEqual(first->bstrVal, second->bstrVal);
				
				::VariantClear(&variant);
			}();

			[]() // Copy assignment operator
			{
				VARIANT variant;
				::VariantInit(&variant);

				variant.vt = VT_BSTR;
				variant.bstrVal = ::SysAllocString(L"Other string");

				auto first = Com::Variant(&variant);
				auto second = Com::Variant();

				second = first;

				Assert::IsTrue(VT_BSTR == first->vt);
				Assert::IsTrue(VT_BSTR == second->vt);

				Assert::AreEqual(L"Other string", first->bstrVal);
				Assert::AreEqual(L"Other string", second->bstrVal);

				Assert::AreNotEqual(first->bstrVal, second->bstrVal);
				
				::VariantClear(&variant);
			}();

			[]() // Move constructor
			{
				VARIANT variant;
				::VariantInit(&variant);

				variant.vt = VT_BSTR;
				variant.bstrVal = ::SysAllocString(L"Some string");

				auto first = Com::Variant(&variant);
				auto second = std::move(first);

				Assert::IsTrue(VT_EMPTY == first->vt);
				Assert::IsTrue(VT_BSTR == second->vt);

				Assert::AreEqual(L"Some string", second->bstrVal);
				
				::VariantClear(&variant);
			}();

			[]() // Move assignment operator
			{
				VARIANT variant;
				::VariantInit(&variant);

				variant.vt = VT_BSTR;
				variant.bstrVal = ::SysAllocString(L"Other string");

				auto first = Com::Variant(&variant);
				auto second = Com::Variant();

				second = std::move(first);

				Assert::IsTrue(VT_EMPTY == first->vt);
				Assert::IsTrue(VT_BSTR == second->vt);

				Assert::AreEqual(L"Other string", second->bstrVal);
				
				::VariantClear(&variant);
			}();
		}
	};
}