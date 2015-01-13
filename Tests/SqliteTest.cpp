#include "stdafx.h"
#include <limits.h>
#include <WinToolsLib\Data\Sqlite\Database.h>
#include <WinToolsLib\Data\Sqlite\SqliteException.h>
#include <WinToolsLib\Data\Sqlite\Transaction.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;
using namespace WinToolsLib::Data::Sqlite;

namespace Tests
{
	TEST_CLASS(SqliteTest)
	{
	public:
		TEST_METHOD(SqliteDatabase)
		{
			Assert::ExpectException<SqliteException>([]()
			{
				Database::Open(L"Q:\\Some\\Bad\\Path.db");
			});

			auto db = Database::Open(L":memory:");
			db.Execute(
				L"CREATE TABLE test ("
				L"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				L"name TEXT NOT NULL,"
				L"stamp DATETIME,"
				L"bool_value BOOL,"
				L"int8_value INTEGER,"
				L"uint8_value INTEGER,"
				L"int16_value INTEGER,"
				L"uint16_value INTEGER,"
				L"int32_value INTEGER,"
				L"uint32_value INTEGER,"
				L"int64_value INTEGER,"
				L"uint64_value INTEGER,"
				L"float_value REAL,"
				L"double_value REAL,"
				L"blob_value BLOB);");

			auto insert = db.CompileStatement(
				L"INSERT INTO [test] ([name], [stamp], [bool_value], [int8_value], [uint8_value], [int16_value], [uint16_value]"
				L", [int32_value], [uint32_value], [int64_value], [uint64_value], [float_value], [double_value], [blob_value])"
				L"VALUES (@name, @stamp, @bool_value, @int8_value, @uint8_value, @int16_value, @uint16_value"
				L", @int32_value, @uint32_value, @int64_value, @uint64_value, @float_value, @double_value, @blob_value)");
			
			insert->StoreProperty(L"name", StringW(L"First"));
			insert->StoreNull(L"stamp");
			insert->Execute();

			insert->StoreProperty(L"name", StringW(L"Second"));
			insert->StoreProperty(L"stamp", StringW(L"2012-12-31 23:59:59"));
			insert->Execute();

			insert->StoreProperty(L"name", StringW(L"Third"));
			insert->Execute();

			insert->StoreProperty(L"name", StringW(L"Fourth"));
			insert->StoreProperty(L"bool_value", False);
			insert->StoreProperty(L"int8_value", -128);
			insert->StoreProperty(L"uint8_value", 255);
			insert->StoreProperty(L"int16_value", -32768);
			insert->StoreProperty(L"uint16_value", 65536);
			insert->StoreProperty(L"int32_value", -(Int32)2147483648);
			insert->StoreProperty(L"uint32_value", (UInt32)4294967295);
			insert->StoreProperty(L"int64_value", -(Int64)9223372036854775808);
			insert->StoreProperty(L"uint64_value", 18446744073709551610);
			insert->StoreProperty(L"float_value", (Float)2.1);
			insert->StoreProperty(L"double_value", 2.2);

			Int32 *a = new Int32(0x8421C3E9);
			Buffer blob(reinterpret_cast<PCByte>(a), sizeof(Int32));
			insert->StoreProperty(L"blob_value", blob);

			insert->Execute();

			Assert::ExpectException<ColumnNotFoundException>([&insert]()
			{
				insert->StoreProperty(L"random", StringW(L"Some"));
			});

			auto select = db.ExecuteQuery(
				L"SELECT * FROM [test]");

			[&select]()
			{
				auto name = select->LoadProperty(L"name", L"");
				Assert::AreEqual(L"First", name);

				auto stamp = select->LoadProperty(L"stamp", L"2013-01-01 00:00:00");
				Assert::AreEqual(L"2013-01-01 00:00:00", stamp);

				auto null = select->IsNull(L"stamp");
				Assert::IsTrue(null);

				auto next = select->NextRow();
				Assert::IsTrue(next);
			}();

			[&select]()
			{
				auto name = select->LoadProperty(L"name", L"");
				Assert::AreEqual(L"Second", name);

				auto stamp = select->LoadProperty(L"stamp", L"2012-12-31 23:59:59");
				Assert::AreEqual(L"2012-12-31 23:59:59", stamp);

				auto next = select->NextRow();
				Assert::IsTrue(next);
			}();

			[&select]()
			{
				auto name = select->LoadProperty(L"name", L"");
				Assert::AreEqual(L"Third", name);

				auto stamp = select->LoadProperty(L"stamp", L"2012-12-31 23:59:59");
				Assert::AreEqual(L"2012-12-31 23:59:59", stamp);

				auto null = select->IsNull(L"stamp");
				Assert::IsFalse(null);

				auto next = select->NextRow();
				Assert::IsTrue(next);
			}();

			[&select, &blob]()
			{
				auto name = select->LoadProperty(L"name", L"");
				Assert::AreEqual(L"Fourth", name);

				auto bool_value = select->LoadProperty(L"bool_value", True);
				Assert::AreEqual(False, bool_value);

				auto int8_value = select->LoadProperty(L"int8_value", 0);
				Assert::AreEqual(-128, int8_value);

				auto uint8_value = select->LoadProperty(L"uint8_value", 0);
				Assert::AreEqual(255, uint8_value);

				auto int16_value = select->LoadProperty(L"int16_value", 0);
				Assert::AreEqual(-32768, int16_value);

				auto uint16_value = select->LoadProperty(L"uint16_value", 0);
				Assert::AreEqual(65536, uint16_value);

				auto int32_value = select->LoadProperty(L"int32_value", 0);
				Assert::IsTrue(-(Int32)2147483648 == int32_value);

				auto uint32_value = select->LoadProperty(L"uint32_value", (UInt32)0);
				Assert::IsTrue(4294967295 == uint32_value);

				auto int64_value = select->LoadProperty(L"int64_value", (UInt64)0);
				Assert::IsTrue(-(Int64)9223372036854775808 == int64_value);

				auto uint64_value = select->LoadProperty(L"uint64_value", 0);
				Assert::IsTrue(18446744073709551610 == uint64_value);

				auto float_value = select->LoadProperty(L"float_value", (Float)0.0);
				Assert::IsTrue(((Float)2.1) == float_value);

				auto double_value = select->LoadProperty(L"double_value", 0.0);
				Assert::IsTrue(2.2 == double_value);

				Buffer defBlob;
				auto blob_value = select->LoadProperty(L"blob_value", defBlob);
				Assert::IsTrue(Buffer::ToString(blob_value) == Buffer::ToString(blob));

				auto next = select->NextRow();
				Assert::IsFalse(next);
			}();

			Assert::IsTrue(4 == db.ExecuteScalar(
				L"SELECT COUNT(*) FROM [test]"));
			
			Assert::IsTrue(3 == db.ExecuteScalar(
				L"SELECT COUNT(id) FROM [test] WHERE [stamp] IS NOT NULL"));
		}

		TEST_METHOD(SqliteTransaction)
		{
			auto db = Database::Open(L":memory:");
			db.Execute(
				L"CREATE TABLE table1 ("
				L"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				L"value INTEGER NOT NULL);");

			db.Execute(
				L"CREATE TABLE table2 ("
				L"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				L"value INTEGER NOT NULL,"
				L"table1Id INTEGER NOT NULL REFERENCES table1 (Id));");

			{
				Transaction transaction(&db);

				auto insert = db.CompileStatement(L"INSERT INTO [table1] ([value]) VALUES (@value);");
				insert->StoreProperty(L"value", 1);
				insert->Execute();
				auto table1Id = insert->GetLastId();

				insert = db.CompileStatement(L"INSERT INTO [table2] ([value], [table1Id]) VALUES (@value, @table1Id);");
				insert->StoreProperty(L"value", 1);
				insert->StoreProperty(L"table1Id", table1Id);
				insert->Execute();

				transaction.Commit();
			}

			try
			{
				Transaction transaction(&db);

				auto insert = db.CompileStatement(L"INSERT INTO [table1] ([value]) VALUES (@value);");
				insert->StoreProperty(L"value", 2);
				insert->Execute();

				THROW_SQLITE_EXCEPTION(1);

				transaction.Commit();
			}
			catch (SqliteException&)
			{
			}

			{
				Transaction transaction(&db);

				auto insert = db.CompileStatement(L"INSERT INTO [table1] ([value]) VALUES (@value);");
				insert->StoreProperty(L"value", 3);
				insert->Execute();
				auto table1Id = insert->GetLastId();

				insert = db.CompileStatement(L"INSERT INTO [table2] ([value], [table1Id]) VALUES (@value, @table1Id);");
				insert->StoreProperty(L"value", 3);
				insert->StoreProperty(L"table1Id", table1Id);
				insert->Execute();

				transaction.Rollback();
			}

			{
				auto select = db.ExecuteQuery(L"SELECT COUNT(*) count FROM [table1]");
				auto count = select->LoadProperty(L"count", 0);
				Assert::AreEqual(count, 1);

				select = db.ExecuteQuery(L"SELECT COUNT(*) count FROM [table2]");
				count = select->LoadProperty(L"count", 0);
				Assert::AreEqual(count, 1);
			}

			
			{
				auto select1 = db.ExecuteQuery(L"SELECT * FROM [table1]");
				auto select2 = db.ExecuteQuery(L"SELECT * FROM [table2]");
				
				auto value1 = select1->LoadProperty(L"value", 0);
				auto value2 = select2->LoadProperty(L"value", 0);

				Assert::AreEqual(value1, value2);
			}
			

		}
	};
}