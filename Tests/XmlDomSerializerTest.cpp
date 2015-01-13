#include "stdafx.h"
#include <WinToolsLib\String.h>
#include <WinToolsLib\Buffer.h>
#include <WinToolsLib\XmlDom\XmlDom.h>
#include <WinToolsLib\XmlDom\Serializer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(XmlDomSerializerTest)
	{
	public:
		TEST_METHOD(XmlDomSerializerLoad)
		{
			const TChar raw[] = Text("\xfeff")
				Text("<root attrString=\"Some &lt;text&gt;\" attrBlob=\"8421C3E9\" attrInt=\"-123\" attrHex=\"0xAB\" attrFloat=\"0.125\" attrDouble=\"3.1415926535897932\">\r\n")
				Text("\t<item attrTrue=\"True\" attrFalse=\"False\" attrYes=\"Yes\" attrNo=\"No\" attrOne=\"1\" attrZero=\"0\">\r\n")
				Text("\t\t<subitem index=\"0\"/>\r\n")
				Text("\t\t<subitem index=\"1\"/>\r\n")
				Text("\t\t<subitem index=\"2\"/>\r\n")
				Text("\t</item>\r\n")
				Text("</root>\r\n");

			XmlDom::Document doc;
			
			auto success = doc.LoadFromMemory((PCChar)raw, sizeof(raw));
			Assert::AreEqual(True, success);

			auto node = doc.FindChild(Text("root"));
			Assert::IsNotNull(node);

			Serialization::ILoadPropertyPtr root(new XmlDom::Serializer(node));
			[&root]()
			{
				Assert::AreEqual(Text("Some <text>"), root->LoadProperty(Text("attrString"), Text("")));

				Buffer defBlob;
				auto blob = root->LoadProperty(Text("attrBlob"), defBlob);
				Assert::AreEqual(Text("8421C3E9"), Buffer::ToString(blob));

				Assert::AreEqual((Int8)-123, root->LoadProperty(Text("attrInt"), (Int8)-1));
				Assert::AreEqual((Int16)-123, root->LoadProperty(Text("attrInt"), (Int16)-1));
				Assert::AreEqual(-123, root->LoadProperty(Text("attrInt"), 0));
				Assert::AreEqual(-123LL, root->LoadProperty(Text("attrInt"), 0LL));

				Assert::AreEqual((UInt8)0xAB, root->LoadProperty(Text("attrHex"), (UInt8)-1));
				Assert::AreEqual((UInt16)0xAB, root->LoadProperty(Text("attrHex"), (UInt16)-1));
				Assert::AreEqual(0xABU, root->LoadProperty(Text("attrHex"), 0U));
				Assert::AreEqual(0xABULL, root->LoadProperty(Text("attrHex"), 0ULL));
				Assert::AreEqual(0.125F, root->LoadProperty(Text("attrFloat"), 0.0F));
				Assert::AreEqual(3.1415926535897932, root->LoadProperty(Text("attrDouble"), 0.0));

				Assert::AreEqual(True, root->LoadProperty(Text("nonexisting"), True));
				Assert::AreEqual(False, root->LoadProperty(Text("nonexisting"), False));

				auto item = root->LoadChild(Text("item"));
				[&item]()
				{
					Assert::AreEqual(True, item->LoadProperty(Text("attrTrue"), False));
					Assert::AreEqual(False, item->LoadProperty(Text("attrFalse"), True));
					Assert::AreEqual(True, item->LoadProperty(Text("attrYes"), False));
					Assert::AreEqual(False, item->LoadProperty(Text("attrNo"), True));
					Assert::AreEqual(True, item->LoadProperty(Text("attrOne"), False));
					Assert::AreEqual(False, item->LoadProperty(Text("attrZero"), True));

					Int32 index = 0;
					item->LoadChildren(
						Text("subitem"),
						[&index](Serialization::ILoadProperty* subitem)
					{
						Assert::AreEqual(index++, subitem->LoadProperty(Text("index"), -1));
						return True;
					});
				}();
			}();
		}

		TEST_METHOD(XmlDomSerializerStore)
		{
			XmlDom::Document doc;

			auto node = doc.AddChild(Text("root"));
			Assert::IsNotNull(node);

			Int32 *a = new Int32(0x8421C3E9);
			Buffer blob(reinterpret_cast<PCByte>(a), sizeof(Int32));

			Serialization::IStorePropertyPtr root(new XmlDom::Serializer(node));
			[&root, &blob]()
			{
				root->StoreProperty(Text("attrString"), String(Text("Some <text>")));
				root->StoreProperty(Text("attrBlob"), blob);
				root->StoreProperty(Text("attrInt"), -123);
				root->StoreProperty(Text("attrHex"), 0xABu, Serialization::IntFormat::Hex);
				root->StoreProperty(Text("attrFloat"), 0.125F);
				root->StoreProperty(Text("attrDouble"), 3.1415926535897932);

				auto item = root->StoreChild(Text("item"));
				[&item]()
				{
					Assert::ExpectException<XmlDom::UnsupportedFormatException>(
						[&item]()
						{
							item->StoreProperty(Text("attrInt"), -123, Serialization::IntFormat::Hex);
						});

					item->StoreProperty(Text("attrTrue"), True);
					item->StoreProperty(Text("attrFalse"), False);
					item->StoreProperty(Text("attrYes"), True, Serialization::BoolFormat::YesNo);
					item->StoreProperty(Text("attrNo"), False, Serialization::BoolFormat::YesNo);
					item->StoreProperty(Text("attrOne"), True, Serialization::BoolFormat::OneZero);
					item->StoreProperty(Text("attrZero"), False, Serialization::BoolFormat::OneZero);

					for (auto i = 0; i < 3; i++)
					{
						auto subitem = item->StoreChild(Text("subitem"));
						subitem->StoreProperty(Text("index"), i);
					}
				}();
			}();

			String output;
			doc.Save(output, XmlDom::UTF_16);

			const String expected = String::FormatS(
				Text("<?xml version=\"1.0\" encoding=\"UTF-16\"?>\r\n")
				Text("<root attrString=\"Some &lt;text&gt;\" attrBlob=\"%s\" attrInt=\"-123\" attrHex=\"0xAB\" attrFloat=\"0.125\" attrDouble=\"3.1415926535897931\">\r\n")
				Text("\t<item attrTrue=\"True\" attrFalse=\"False\" attrYes=\"Yes\" attrNo=\"No\" attrOne=\"1\" attrZero=\"0\">\r\n")
				Text("\t\t<subitem index=\"0\"/>\r\n")
				Text("\t\t<subitem index=\"1\"/>\r\n")
				Text("\t\t<subitem index=\"2\"/>\r\n")
				Text("\t</item>\r\n")
				Text("</root>\r\n"),
				Buffer::ToString(blob).GetBuffer());

			Assert::AreEqual(expected, output.GetBuffer());
		}
	};
}