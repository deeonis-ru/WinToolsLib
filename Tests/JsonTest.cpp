#include "stdafx.h"
#include <WinToolsLib\String.h>
#include <WinToolsLib\Data\Json\json.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(JsonTest)
	{
	public:
		TEST_METHOD(JsonParse)
		{
			{
				const auto input = "{\"status\": \"ok\", \"results\": [{\"recordings\": [{\"id\": \"AF3F6726-57FD-49DF-B11E-2291D9C77657\"}], \"score\": 0.876543, \"complete\": true}]}";

				auto json = jsoncons::json::parse_string(input);
				Assert::IsTrue(json.is_object());

				auto& status = json.get("status");
				Assert::IsTrue(status.is_string());
				Assert::AreEqual("ok", status.as_string().c_str());

				auto& results = json.get("results");
				Assert::IsTrue(results.is_array());

				auto& result1 = results[0];
				Assert::IsTrue(result1.is_object());

				auto& recordings = result1.get("recordings");
				Assert::IsTrue(recordings.is_array());

				auto& recording1 = recordings[0];
				Assert::IsTrue(recording1.is_object());

				auto& id = recording1.get("id");
				Assert::IsTrue(id.is_string());
				Assert::AreEqual("AF3F6726-57FD-49DF-B11E-2291D9C77657", id.as_string().c_str());

				auto& score = result1.get("score");
				Assert::IsTrue(score.is_double());
				Assert::AreEqual(0.876543, score.as_double());

				auto& complete = result1.get("complete");
				Assert::IsTrue(complete.is_bool());
				Assert::AreEqual(true, complete.as_bool());
			}
			{
				Assert::ExpectException<jsoncons::json_exception>([]()
				{
					const auto input = "{\"status\": \"ok\", junk \"results\": [{\"recordings\": [{\"id\": \"AF3F6726-57FD-49DF-B11E-2291D9C77657\"}], \"score\": 0.876543, \"complete\": true}]}";
					jsoncons::json::parse_string(input);
				});
			}
			{
				Assert::ExpectException<jsoncons::json_exception>([]()
				{
					const auto input = "{\"status\": \"ok\", \"results\": [{\"recordings\": [{\"id\": \"AF3F6726-57FD-49DF-B11E-2291D9C77657\"}], withoutquots: 0.876543, \"complete\": true}]}";
					jsoncons::json::parse_string(input);
				});
			}
			{
				Assert::ExpectException<jsoncons::json_exception>([]()
				{
					const auto input = "[ true, fals";
					jsoncons::json::parse_string(input);
				});
			}
			{
				const auto input =
					"["
					"    {"
					"        \"title\" : \"Kafka on the Shore\","
					"        \"author\" : \"Haruki Murakami\","
					"        \"price\" : 25.17,"
					"        \"reviews\" :"
					"        ["
					"            {\"rating\" : \"*****\"},"
					"            {\"rating\" : \"*****\"}"
					"        ]"
					"    },"
					"    {"
					"        \"title\" : \"Women: A Novel\","
					"        \"author\" : \"Charles Bukowski\","
					"        \"price\" : 12.00,"
					"        \"reviews\" :"
					"        ["
					"            {\"rating\" : \"*****\"},"
					"            {\"rating\" : \"*\"}"
					"        ]"
					"    },"
					"    {"
					"        \"title\" : \"Cutter's Way\","
					"        \"author\" : \"Ivan Passer\","
					"        \"reviews\" :"
					"        ["
					"            {\"rating\" : \"****\"}"
					"        ]"
					"    }"
					"]";

				auto json = jsoncons::json::parse_string(input);
				Assert::IsTrue(json.is_array());
				Assert::IsTrue(json.size() == 3);

				auto& book = json[2];
				Assert::IsTrue(book.is_object());
				Assert::IsTrue(book.has_member("author"));
				Assert::IsTrue(book["title"].as_string() == "Cutter's Way");
				Assert::IsTrue(book["reviews"].is_array());
			}
		}

		TEST_METHOD(JsonConstruct)
		{
			using namespace jsoncons;
			{
				json books(json::an_array);

				{
					json book(json::an_object);
					book["title"] = "Kafka on the Shore";
					book["author"] = "Haruki Murakami";
					book["price"] = 25.17;
					books.add(std::move(book));
				}
				{
					json book(json::an_object);
					book["title"] = "Women: A Novel";
					book["author"] = "Charles Bukowski";
					book["price"] = 12.00;
					books.add(std::move(book));
				}
				{
					json book(json::an_object);
					book["title"] = "Cutter's Way";
					book["author"] = "Ivan Passer";
					books.add(std::move(book));
				}

				const auto output = books.to_string();
				const auto expected = "[{\"title\":\"Kafka on the Shore\",\"author\":\"Haruki Murakami\",\"price\":25.17},{\"title\":\"Women: A Novel\",\"author\":\"Charles Bukowski\",\"price\":12.0},{\"title\":\"Cutter's Way\",\"author\":\"Ivan Passer\"}]";
				Assert::AreEqual(expected, output.c_str());
			}
		}

		TEST_METHOD(JsonEscape)
		{
			using namespace jsoncons;
			{
				json json("C:\\Windows\\System32\\calc.exe");

				const auto output = json.to_string();
				const auto expected = "\"C:\\\\Windows\\\\System32\\\\calc.exe\"";
				Assert::AreEqual(expected, output.c_str());
			}
			{
				json json("first line\nsecond line");

				const auto output = json.to_string();
				const auto expected = "\"first line\\nsecond line\"";
				Assert::AreEqual(expected, output.c_str());
			}
		}

		TEST_METHOD(JsonUtf8)
		{
			using namespace jsoncons;
			using namespace WinToolsLib;
			{
				const auto utf16 = StringW(L"text текст");
				const auto utf8 = StringB::Convert(utf16);

				json root(json::an_object);
				root["utf8"] = (Char*)utf8.GetBuffer();

				const auto output = root.to_string();
				const auto expected = "{\"utf8\":\"text \xD1\x82\xD0\xB5\xD0\xBA\xD1\x81\xD1\x82\"}";
				Assert::AreEqual(expected, output.c_str());
			}
			{
				const auto utf16 = StringW(L"text текст");
				const auto utf8 = StringB::Convert(utf16);

				json root(json::an_object);
				root["utf8"] = (Char*)utf8.GetBuffer();
				
				output_format format;
				format.escape_all_non_ascii(True);

				const auto output = root.to_string(format);
				const auto expected = "{\"utf8\":\"text \\u0442\\u0435\\u043A\\u0441\\u0442\"}";
				Assert::AreEqual(expected, output.c_str());
			}
		}
	};
}