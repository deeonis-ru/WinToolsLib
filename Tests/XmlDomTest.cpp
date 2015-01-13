#include "stdafx.h"
#include <limits.h>
#include <WinToolsLib\XmlDom\XmlDom.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<XmlDom::NodeType>(const XmlDom::NodeType& type)
			{
				switch (type)
				{
				case XmlDom::DOCUMENT:
					return std::wstring(L"DOCUMENT");
				case XmlDom::ELEMENT:
					return std::wstring(L"ELEMENT");
				case XmlDom::COMMENT:
					return std::wstring(L"COMMENT");
				case XmlDom::DECLARATION:
					return std::wstring(L"DECLARATION");
				default:
					return std::wstring(L"Unknown");
				}
			}
		}
	}
}

namespace Tests
{
	TEST_CLASS(XmlDomTest)
	{
	public:
		TEST_METHOD(XmlDomBaseGetSetName)
		{
			XmlDom::Base base;
			Assert::AreEqual(Text(""), base.GetName());
			
			base.SetName(Text("Name"));
			Assert::AreEqual(Text("Name"), base.GetName());
			
			base.SetName(Text("Another name"));
			Assert::AreEqual(Text("Another name"), base.GetName());
		}

		TEST_METHOD(XmlDomBaseGetSetString)
		{
			XmlDom::Base base;
			Assert::AreEqual(Text(""), base.GetString());
			
			base.SetString(Text("Value"));
			Assert::AreEqual(Text("Value"), base.GetString());
			
			base.SetString(Text("Another value"));
			Assert::AreEqual(Text("Another value"), base.GetString());
		}

		TEST_METHOD(XmlDomBaseGetSetBool)
		{
			XmlDom::Base base;
			Assert::ExpectException<XmlDom::BadAttributeValueException>(
				[&base]()
				{
					base.GetBool();
				});

			base.SetString(Text("True"));
			Assert::AreEqual(True, base.GetBool());

			base.SetString(Text("false"));
			Assert::AreEqual(False, base.GetBool());

			base.SetString(Text("Yes"));
			Assert::AreEqual(True, base.GetBool());

			base.SetString(Text("NO"));
			Assert::AreEqual(False, base.GetBool());

			base.SetString(Text("1"));
			Assert::AreEqual(True, base.GetBool());

			base.SetString(Text("0"));
			Assert::AreEqual(False, base.GetBool());

			base.SetString(Text("Bad"));
			Assert::ExpectException<XmlDom::BadAttributeValueException>(
				[&base]()
				{
					base.GetBool();
				});

			base.SetBool(True);
			Assert::AreEqual(True, base.GetBool());
			Assert::AreEqual(Text("True"), base.GetString());

			base.SetBool(False);
			Assert::AreEqual(False, base.GetBool());
			Assert::AreEqual(Text("False"), base.GetString());

			base.SetBool(True, Serialization::BoolFormat::TrueFalse);
			Assert::AreEqual(True, base.GetBool());
			Assert::AreEqual(Text("True"), base.GetString());

			base.SetBool(False, Serialization::BoolFormat::TrueFalse);
			Assert::AreEqual(False, base.GetBool());
			Assert::AreEqual(Text("False"), base.GetString());

			base.SetBool(True, Serialization::BoolFormat::YesNo);
			Assert::AreEqual(True, base.GetBool());
			Assert::AreEqual(Text("Yes"), base.GetString());

			base.SetBool(False, Serialization::BoolFormat::YesNo);
			Assert::AreEqual(False, base.GetBool());
			Assert::AreEqual(Text("No"), base.GetString());

			base.SetBool(True, Serialization::BoolFormat::OneZero);
			Assert::AreEqual(True, base.GetBool());
			Assert::AreEqual(Text("1"), base.GetString());

			base.SetBool(False, Serialization::BoolFormat::OneZero);
			Assert::AreEqual(False, base.GetBool());
			Assert::AreEqual(Text("0"), base.GetString());
		}

		TEST_METHOD(XmlDomBaseGetSetInt32)
		{
			XmlDom::Base base;
			Assert::AreEqual(0, base.GetInt32());
			
			base.SetInt32(_I32_MIN);
			Assert::AreEqual(_I32_MIN, base.GetInt32());
			Assert::AreEqual(Text("-2147483648"), base.GetString());
			
			base.SetInt32(_I32_MAX);
			Assert::AreEqual(_I32_MAX, base.GetInt32());
			Assert::AreEqual(Text("2147483647"), base.GetString());
		}

		TEST_METHOD(XmlDomBaseGetSetInt64)
		{
			XmlDom::Base base;
			Assert::AreEqual(0LL, base.GetInt64());
			
			base.SetInt64(_I64_MIN);
			Assert::AreEqual(_I64_MIN, base.GetInt64());
			Assert::AreEqual(Text("-9223372036854775808"), base.GetString());
			
			base.SetInt64(_I64_MAX);
			Assert::AreEqual(_I64_MAX, base.GetInt64());
			Assert::AreEqual(Text("9223372036854775807"), base.GetString());
		}

		TEST_METHOD(XmlDomBaseGetSetUInt32)
		{
			XmlDom::Base base;
			Assert::AreEqual(0U, base.GetUInt32());
			
			base.SetUInt32(_UI32_MAX);
			Assert::AreEqual(_UI32_MAX, base.GetUInt32());
			Assert::AreEqual(Text("4294967295"), base.GetString());
			
			base.SetUInt32(_UI32_MAX, XmlDom::IntFormat::Hex);
			Assert::AreEqual(_UI32_MAX, base.GetUInt32());
			Assert::AreEqual(Text("0xFFFFFFFF"), base.GetString());
		}

		TEST_METHOD(XmlDomBaseGetSetUInt64)
		{
			XmlDom::Base base;
			Assert::AreEqual(0ULL, base.GetUInt64());
			
			base.SetUInt64(_UI64_MAX);
			Assert::AreEqual(_UI64_MAX, base.GetUInt64());
			Assert::AreEqual(Text("18446744073709551615"), base.GetString());
			
			base.SetUInt64(_UI64_MAX, XmlDom::IntFormat::Hex);
			Assert::AreEqual(_UI64_MAX, base.GetUInt64());
			Assert::AreEqual(Text("0xFFFFFFFFFFFFFFFF"), base.GetString());
		}

		TEST_METHOD(XmlDomBaseGetSetFloat)
		{
			XmlDom::Base base;
			Assert::AreEqual(0.0F, base.GetFloat());
			
			base.SetFloat(FLT_MIN);
			Assert::AreEqual(FLT_MIN, base.GetFloat());
			Assert::AreEqual(Text("1.175494351e-038"), base.GetString());
			
			base.SetFloat(FLT_MAX);
			Assert::AreEqual(FLT_MAX, base.GetFloat());
			Assert::AreEqual(Text("3.402823466e+038"), base.GetString());
		}

		TEST_METHOD(XmlDomBaseGetSetDouble)
		{
			XmlDom::Base base;
			Assert::AreEqual(0.0, base.GetDouble());
			
			base.SetDouble(DBL_MIN);
			Assert::AreEqual(DBL_MIN, base.GetDouble());
			Assert::AreEqual(Text("2.2250738585072014e-308"), base.GetString());
			
			base.SetDouble(DBL_MAX);
			Assert::AreEqual(DBL_MAX, base.GetDouble());
			Assert::AreEqual(Text("1.7976931348623157e+308"), base.GetString());
		}

		TEST_METHOD(XmlDomNode)
		{
			const TChar raw[] = Text("\xfeff")
				Text("<root>\r\n")
				Text("\t<!--Some comment goes here-->\r\n")
				Text("\t<item attrInt=\"2147483647\" attrTrue=\"1\" attrFalse=\"0\">\r\n")
				Text("\t\t<!--Another comment!-->\r\n")
				Text("\t\t<subitem attrHex=\"0x0200\" attrFloat=\"-1.0001\" attrDouble=\"3.33333\"/>\r\n")
				Text("\t\t<!--One more comment-->\r\n")
				Text("\t</item>\r\n")
				Text("<!--The last one-->\r\n")
				Text("</root>\r\n");

			XmlDom::Document doc;
			
			auto success = doc.LoadFromMemory((PCChar)raw, sizeof(raw));
			Assert::AreEqual(True, success);
			Assert::AreEqual(True, doc.HasChild());
			Assert::AreEqual((SizeT)1, doc.GetChildCount());
			Assert::AreEqual(XmlDom::DOCUMENT, doc.GetType());

			auto root = doc.FindChild(Text("root"));
			Assert::IsNotNull(root);
			Assert::AreEqual(XmlDom::ELEMENT, root->GetType());
			Assert::AreEqual((SizeT)3, root->GetChildCount());
			Assert::AreEqual((SizeT)1, root->GetChildCount(Text("item")));

			auto item = root->FindChild(Text("item"));
			Assert::IsNotNull(item);
			Assert::AreEqual(True, item->HasChild());
			Assert::AreEqual(True, item->HasAttribute());

			auto nonexistingChild = root->FindChild(Text("nonexisting"));
			Assert::IsNull(nonexistingChild);

			Assert::AreEqual(True, item->ReadAttributeAsBool(Text("attrTrue")));
			Assert::AreEqual(True, item->ReadAttributeAsBool(Text("attrTrue"), True));
			Assert::AreEqual(True, item->ReadAttributeAsBool(Text("nonexisting"), True));
			Assert::AreEqual(False, item->ReadAttributeAsBool(Text("attrFalse")));
			Assert::AreEqual(False, item->ReadAttributeAsBool(Text("nonexisting")));

			Assert::AreEqual(Text("1"), item->ReadAttributeAsString(Text("attrTrue")));
			Assert::AreEqual(Text("0"), item->ReadAttributeAsString(Text("attrFalse")));
			Assert::AreEqual(Text("Qwerty"), item->ReadAttributeAsString(Text("nonexisting"), Text("Qwerty")));

			Assert::AreEqual(2147483647, item->ReadAttributeAsInt32(Text("attrInt")));
			Assert::AreEqual(2147483647, item->ReadAttributeAsInt32(Text("nonexisting"), 2147483647));
			Assert::AreEqual(2147483647.0, item->ReadAttributeAsDouble(Text("attrInt")));
			Assert::AreEqual(2147483647.0, item->ReadAttributeAsDouble(Text("nonexisting"), 2147483647.0));
			Assert::AreEqual(2147483647.0f, item->ReadAttributeAsFloat(Text("attrInt")));
			Assert::AreEqual(2147483647.0f, item->ReadAttributeAsFloat(Text("nonexisting"), 2147483647.0f));

			auto attrFalse = item->FindAttribute(Text("attrFalse"));
			Assert::IsNotNull(attrFalse);
			Assert::AreEqual(False, attrFalse->GetBool());

			auto nonexistingAttr = item->FindAttribute(Text("nonexisting"));
			Assert::IsNull(nonexistingAttr);

			item->RemoveAttribute(attrFalse);

			XmlDom::AttributeIterator attrIter;

			auto attrInt = item->GetFirstAttribute(attrIter);
			Assert::IsNotNull(attrInt);

			auto attrTrue = item->GetNextAttribute(attrIter);
			Assert::IsNotNull(attrTrue);

			auto attrNull = item->GetNextAttribute(attrIter);
			Assert::IsNull(attrNull);

			item->ClearAttribute();
			Assert::AreEqual(False, item->HasAttribute());

			item->AddAttribute(Text("attrNew"), 7/13);
			Assert::AreEqual(True, item->HasAttribute());

			auto attrNew = item->FindAttribute(Text("attrNew"));
			Assert::IsNotNull(attrNew);

			XmlDom::NodeIterator childIter;

			auto anotherComment = item->GetFirstChild(childIter);
			Assert::IsNotNull(anotherComment);
			Assert::AreEqual(XmlDom::COMMENT, anotherComment->GetType());

			auto subItem = item->GetNextChild(childIter);
			Assert::IsNotNull(subItem);
			Assert::AreEqual(XmlDom::ELEMENT, subItem->GetType());

			[subItem]()
			{
				XmlDom::NodeIterator childIter;
				Assert::IsNull(subItem->GetFirstChild(childIter));
				Assert::AreEqual(True, subItem->IsEmpty());

				Assert::AreEqual(1234u, subItem->ReadAttributeAsUInt32(Text("nonexisting"), 1234u));
				Assert::AreEqual(0x0200u, subItem->ReadAttributeAsUInt32(Text("attrHex")));
			}();

			auto oneMoreComment = item->GetNextChild(childIter);
			Assert::IsNotNull(oneMoreComment);
			Assert::AreEqual(XmlDom::COMMENT, oneMoreComment->GetType());

			item->RemoveChild(subItem);
			item->RemoveChild(anotherComment);
			item->RemoveChild(oneMoreComment);

			Assert::AreEqual(False, item->HasChild());
		}

		TEST_METHOD(XmlDomSave)
		{
			XmlDom::Document doc;

			auto root = doc.AddChild(Text("Root"));
			Assert::IsNotNull(root);

			auto comment = root->AddChild(Text("Some comment goes here"), XmlDom::COMMENT);
			Assert::IsNotNull(comment);

			auto item1 = root->AddChild(Text("Item"));
			Assert::IsNotNull(item1);

			[item1]()
			{
				auto attrInt = item1->AddAttribute(Text("attrInt"), 2147483647);
				Assert::IsNotNull(attrInt);

				auto attrTrue = item1->AddAttribute(Text("attrTrue"), True);
				Assert::IsNotNull(attrTrue);

				auto attrFalse = item1->AddAttribute(Text("attrFalse"), False);
				Assert::IsNotNull(attrFalse);

				auto subItem1 = item1->AddChild(Text("SubItem"));
				Assert::IsNotNull(subItem1);

				[subItem1]()
				{
					auto attrHex = subItem1->AddAttribute(Text("attrHex"));
					Assert::IsNotNull(attrHex);

					attrHex->SetUInt32(512, XmlDom::IntFormat::Hex);

					auto attrString = subItem1->AddAttribute(Text("attrString"), Text("Some <text>"));
					Assert::IsNotNull(attrString);
				}();

				auto subItem2 = item1->AddChild(Text("SubItem"));
				Assert::IsNotNull(subItem2);

				[subItem2]()
				{
					auto attrHex = subItem2->AddAttribute(Text("attrHex"));
					Assert::IsNotNull(attrHex);

					attrHex->SetUInt32(0xABC, XmlDom::IntFormat::Hex);

					auto attrString = subItem2->AddAttribute(Text("attrString"), Text("& Another \"text\""));
					Assert::IsNotNull(attrString);
				}();
			}();
			
			auto item2 = root->AddChild(Text("Item"));
			Assert::IsNotNull(item2);

			[item2]()
			{
				auto attrInt = item2->AddAttribute(Text("attrInt"), -2147483647);
				Assert::IsNotNull(attrInt);

				auto attrFloat = item2->AddAttribute(Text("attrDouble"), 0.0);
				Assert::IsNotNull(attrFloat);
			}();
			
			String output;
			doc.Save(output, XmlDom::UTF_16);

			const TChar expected[] =
				Text("<?xml version=\"1.0\" encoding=\"UTF-16\"?>\r\n")
				Text("<Root>\r\n")
				Text("\t<!--Some comment goes here-->\r\n")
				Text("\t<Item attrInt=\"2147483647\" attrTrue=\"True\" attrFalse=\"False\">\r\n")
				Text("\t\t<SubItem attrHex=\"0x200\" attrString=\"Some &lt;text&gt;\"/>\r\n")
				Text("\t\t<SubItem attrHex=\"0xABC\" attrString=\"&amp; Another &quot;text&quot;\"/>\r\n")
				Text("\t</Item>\r\n")
				Text("\t<Item attrInt=\"-2147483647\" attrDouble=\"0\"/>\r\n")
				Text("</Root>\r\n");

			Assert::AreEqual(expected, output.GetBuffer());
		}

		TEST_METHOD(XmlDomEntities)
		{
			const TChar raw[] = Text("\xfeff")
				Text("<?xml version=\"1.0\" encoding=\"UTF-16\"?>\r\n")
				Text("<root attr=\"ProductName&amp;copy;\">\r\n")
				Text("\t<!--&lt;Some comment with <entities> goes here&gt;-->\r\n")
				Text("\t<item attr=\"&quot;Quoted text&quot;\">\r\n")
				Text("\t\t<subItem attr=\"Some &lt;text&gt;\"/>\r\n")
				Text("\t\t<!--\r\n")
				Text("\t\t<subItem attr=\"&qout;Quoted&reg; text&qout;\"/>\r\n")
				Text("\t\t-->\r\n")
				Text("\t\t<subItem attr=\"&amp; another &apos;text&apos;\"/>\r\n")
				Text("\t</item>\r\n")
				Text("\t<!--Other comment & entity&para;-->\r\n")
				Text("</root>\r\n");

			XmlDom::Document doc;
			
			auto success = doc.LoadFromMemory((PCChar)raw, sizeof(raw));
			Assert::AreEqual(True, success);

			XmlDom::NodeIterator iter;

			auto root = doc.FindFirstChild(Text("root"), iter);
			Assert::IsNotNull(root);
			Assert::AreEqual((SizeT)3, root->GetChildCount());

			[root]()
			{
				Assert::AreEqual(Text("ProductName&copy;"), root->ReadAttributeAsString(Text("attr"), Text("")));

				XmlDom::NodeIterator iter;

				auto item = root->FindFirstChild(Text("item"), iter);
				Assert::IsNotNull(item);
				Assert::AreEqual((SizeT)3, item->GetChildCount());

				[item]()
				{
					Assert::AreEqual(Text("\"Quoted text\""), item->ReadAttributeAsString(Text("attr"), Text("")));

					XmlDom::NodeIterator iter;

					auto subItem1 = item->FindFirstChild(Text("subItem"), iter);
					Assert::IsNotNull(subItem1);
					Assert::AreEqual(Text("Some <text>"), subItem1->ReadAttributeAsString(Text("attr"), Text("")));

					auto subItem2 = item->FindNextChild(Text("subItem"), iter);
					Assert::IsNotNull(subItem2);
					Assert::AreEqual(Text("& another 'text'"), subItem2->ReadAttributeAsString(Text("attr"), Text("")));
				}();
			}();

			String output;
			doc.Save(output, XmlDom::UTF_16);

			const TChar expected[] =
				Text("<?xml version=\"1.0\" encoding=\"UTF-16\"?>\r\n")
				Text("<root attr=\"ProductName&amp;copy;\">\r\n")
				Text("\t<!--&lt;Some comment with <entities> goes here&gt;-->\r\n")
				Text("\t<item attr=\"&quot;Quoted text&quot;\">\r\n")
				Text("\t\t<subItem attr=\"Some &lt;text&gt;\"/>\r\n")
				Text("\t\t<!--\r\n")
				Text("\t\t<subItem attr=\"&qout;Quoted&reg; text&qout;\"/>\r\n")
				Text("\t\t-->\r\n")
				Text("\t\t<subItem attr=\"&amp; another &apos;text&apos;\"/>\r\n")
				Text("\t</item>\r\n")
				Text("\t<!--Other comment & entity&para;-->\r\n")
				Text("</root>\r\n");

			Assert::AreEqual(expected, output.GetBuffer());
		}
	};
}