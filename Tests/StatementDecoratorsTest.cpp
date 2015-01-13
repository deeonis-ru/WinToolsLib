#include "stdafx.h"
#include <limits.h>
#include <WinToolsLib\Data\Sqlite\Database.h>
#include <WinToolsLib\Data\Sqlite\SqliteException.h>
#include <WinToolsLib\Data\Sqlite\StatementDecorators\ExtendableInsert.h>
#include <WinToolsLib\Data\Sqlite\StatementDecorators\ExtendableUpdate.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;
using namespace WinToolsLib::Data::Sqlite;

namespace Tests
{
	TEST_CLASS(StatementDecoratorsTest)
	{
	public:
		TEST_METHOD(ExtendableInsertStatement)
		{
			auto db = Database::Open(L":memory:");

			db.Execute(
				L"CREATE TABLE test ("
				L"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				L"field1 INTEGER);");

			[&db]()
			{
				auto insertSql = String(L"INSERT INTO [test] ([field1])"
					L"VALUES (@field1)");
				auto insert = db.CompileStatement(insertSql);

				StatementDecorators::ExtendableInsert statement(&db, std::move(insert), insertSql);
			
				statement.StoreProperty(L"field1", 22);
				statement.StoreProperty(L"boolField", True);
				statement.StoreProperty(L"int8Field", -16);
				statement.StoreProperty(L"uint8Field", 16);
				statement.StoreProperty(L"int16Field", -512);
				statement.StoreProperty(L"uint16Field", 512);
				statement.StoreProperty(L"int32Field", -131072);
				statement.StoreProperty(L"uint32Field", 131072);
				statement.StoreProperty(L"int64Field", -8589934592);
				statement.StoreProperty(L"uint64Field", 8589934592);
				statement.StoreProperty(L"floatField", 100.61);
				statement.StoreProperty(L"doubleField", 10000.61);
				statement.StoreProperty(L"stringField", String(L"Some"));
				statement.StoreNull(L"nullField");
				statement.Execute();

				auto select = db.ExecuteQuery(
					L"SELECT * FROM [test] WHERE Id = 1");

				auto field1 = select->LoadProperty(L"field1", 0);
				Assert::AreEqual(22, field1);

				auto boolField = select->LoadProperty(L"boolField", False);
				Assert::AreEqual(True, boolField);

				auto int8Field = select->LoadProperty(L"int8Field", (Int8)0);
				Assert::IsTrue(-16 == int8Field);

				auto uint8Field = select->LoadProperty(L"uint8Field", (UInt8)0);
				Assert::IsTrue(16 == uint8Field);

				auto int16Field = select->LoadProperty(L"int16Field", (Int16)0);
				Assert::IsTrue(-512 == int16Field);

				auto uint16Field = select->LoadProperty(L"uint16Field", (UInt16)0);
				Assert::IsTrue(512 == uint16Field);

				auto int32Field = select->LoadProperty(L"int32Field", (Int32)0);
				Assert::IsTrue(-131072 == int32Field);

				auto uint32Field = select->LoadProperty(L"uint32Field", (UInt32)0);
				Assert::IsTrue(131072 == uint32Field);

				auto int64Field = select->LoadProperty(L"int64Field", (Int64)0);
				Assert::IsTrue(-8589934592 == int64Field);

				auto uint64Field = select->LoadProperty(L"uint64Field", (UInt64)0);
				Assert::IsTrue(8589934592 == uint64Field);

				auto floatField = select->LoadProperty(L"floatField", (Float)0);
				Assert::IsTrue(0 != floatField);

				auto doubleField = select->LoadProperty(L"doubleField", (Double)0);
				Assert::IsTrue(0 != doubleField);

				auto stringField = select->LoadProperty(L"stringField", Text(""));
				Assert::AreEqual(L"Some", stringField);

				auto nullField = select->LoadProperty(L"nullField", 0);
				Assert::IsTrue(NULL == nullField);
			}();

			// without not specified fields
			[&db]()
			{
				auto insertSql = String(L"INSERT INTO [test] ([field1])"
					L"VALUES (23)");
				auto insert = db.CompileStatement(insertSql);

				StatementDecorators::ExtendableInsert statement(&db, std::move(insert), insertSql);
			
				statement.StoreProperty(L"boolField", True);
				statement.StoreProperty(L"int8Field", -16);
				statement.StoreProperty(L"uint8Field", 16);
				statement.StoreProperty(L"int16Field", -512);
				statement.StoreProperty(L"uint16Field", 512);
				statement.StoreProperty(L"int32Field", -131072);
				statement.StoreProperty(L"uint32Field", 131072);
				statement.StoreProperty(L"int64Field", -8589934592);
				statement.StoreProperty(L"uint64Field", 8589934592);
				statement.StoreProperty(L"floatField", 100.61);
				statement.StoreProperty(L"doubleField", 10000.61);
				statement.StoreProperty(L"stringField", String(L"Some"));
				statement.StoreNull(L"nullField");
				statement.Execute();

				auto select = db.ExecuteQuery(
					L"SELECT * FROM [test] WHERE Id = 2");


				auto field1 = select->LoadProperty(L"field1", 0);
				Assert::AreEqual(23, field1);

				auto boolField = select->LoadProperty(L"boolField", False);
				Assert::AreEqual(True, boolField);

				auto int8Field = select->LoadProperty(L"int8Field", (Int8)0);
				Assert::IsTrue(-16 == int8Field);

				auto uint8Field = select->LoadProperty(L"uint8Field", (UInt8)0);
				Assert::IsTrue(16 == uint8Field);

				auto int16Field = select->LoadProperty(L"int16Field", (Int16)0);
				Assert::IsTrue(-512 == int16Field);

				auto uint16Field = select->LoadProperty(L"uint16Field", (UInt16)0);
				Assert::IsTrue(512 == uint16Field);

				auto int32Field = select->LoadProperty(L"int32Field", (Int32)0);
				Assert::IsTrue(-131072 == int32Field);

				auto uint32Field = select->LoadProperty(L"uint32Field", (UInt32)0);
				Assert::IsTrue(131072 == uint32Field);

				auto int64Field = select->LoadProperty(L"int64Field", (Int64)0);
				Assert::IsTrue(-8589934592 == int64Field);

				auto uint64Field = select->LoadProperty(L"uint64Field", (UInt64)0);
				Assert::IsTrue(8589934592 == uint64Field);

				auto floatField = select->LoadProperty(L"floatField", (Float)0);
				Assert::IsTrue(0 != floatField);

				auto doubleField = select->LoadProperty(L"doubleField", (Double)0);
				Assert::IsTrue(0 != doubleField);

				auto stringField = select->LoadProperty(L"stringField", Text(""));
				Assert::AreEqual(L"Some", stringField);

				auto nullField = select->LoadProperty(L"nullField", 0);
				Assert::IsTrue(NULL == nullField);
			}();
		}

		TEST_METHOD(ExtendableUpdateStatement)
		{
			auto db = Database::Open(L":memory:");

			db.Execute(
				L"CREATE TABLE test ("
				L"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				L"field1 INTEGER,"
				L"boolField BOOL,"
				L"int8Field INTEGER,"
				L"uint8Field INTEGER,"
				L"int16Field INTEGER,"
				L"uint16Field INTEGER,"
				L"int32Field INTEGER,"
				L"uint32Field INTEGER,"
				L"int64Field INTEGER,"
				L"uint64Field INTEGER,"
				L"floatField FLOAT,"
				L"doubleField FLOAT,"
				L"stringField TEXT,"
				L"nullField INTEGER);");

			[&db]()
			{
				db.Execute(
					L"INSERT INTO [test] ([field1],[boolField],[int8Field],[uint8Field],[int16Field],[uint16Field],[int32Field],"
					L"[uint32Field],[int64Field],[uint64Field],[floatField],[doubleField],[doubleField],[nullField])"
					L"VALUES (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \"Nothing\", 11)");

				auto updateSql = String(L"UPDATE [test] SET [field1] = @field1 WHERE Id = 1");
				auto update = db.CompileStatement(updateSql);

				StatementDecorators::ExtendableUpdate statement(&db, std::move(update), updateSql);
			
				statement.StoreProperty(L"field1", 22);
				statement.StoreProperty(L"boolField", True);
				statement.StoreProperty(L"int8Field", -16);
				statement.StoreProperty(L"uint8Field", 16);
				statement.StoreProperty(L"int16Field", -512);
				statement.StoreProperty(L"uint16Field", 512);
				statement.StoreProperty(L"int32Field", -131072);
				statement.StoreProperty(L"uint32Field", 131072);
				statement.StoreProperty(L"int64Field", -8589934592);
				statement.StoreProperty(L"uint64Field", 8589934592);
				statement.StoreProperty(L"floatField", 100.61);
				statement.StoreProperty(L"doubleField", 10000.61);
				statement.StoreProperty(L"stringField", String(L"Some"));
				statement.StoreNull(L"nullField");
				statement.Execute();

				auto select = db.ExecuteQuery(
					L"SELECT * FROM [test] WHERE Id = 1");

				auto field1 = select->LoadProperty(L"field1", 0);
				Assert::AreEqual(22, field1);

				auto boolField = select->LoadProperty(L"boolField", False);
				Assert::AreEqual(True, boolField);

				auto int8Field = select->LoadProperty(L"int8Field", (Int8)0);
				Assert::IsTrue(-16 == int8Field);

				auto uint8Field = select->LoadProperty(L"uint8Field", (UInt8)0);
				Assert::IsTrue(16 == uint8Field);

				auto int16Field = select->LoadProperty(L"int16Field", (Int16)0);
				Assert::IsTrue(-512 == int16Field);

				auto uint16Field = select->LoadProperty(L"uint16Field", (UInt16)0);
				Assert::IsTrue(512 == uint16Field);

				auto int32Field = select->LoadProperty(L"int32Field", (Int32)0);
				Assert::IsTrue(-131072 == int32Field);

				auto uint32Field = select->LoadProperty(L"uint32Field", (UInt32)0);
				Assert::IsTrue(131072 == uint32Field);

				auto int64Field = select->LoadProperty(L"int64Field", (Int64)0);
				Assert::IsTrue(-8589934592 == int64Field);

				auto uint64Field = select->LoadProperty(L"uint64Field", (UInt64)0);
				Assert::IsTrue(8589934592 == uint64Field);

				auto floatField = select->LoadProperty(L"floatField", (Float)0);
				Assert::IsTrue(0 != floatField);

				auto doubleField = select->LoadProperty(L"doubleField", (Double)0);
				Assert::IsTrue(0 != doubleField);

				auto stringField = select->LoadProperty(L"stringField", Text(""));
				Assert::AreEqual(L"Some", stringField);

				auto nullField = select->LoadProperty(L"nullField", 0);
				Assert::IsTrue(NULL == nullField);
			}();

			// without not specified fields
			[&db]()
			{
				db.Execute(
					L"INSERT INTO [test] ([field1],[boolField],[int8Field],[uint8Field],[int16Field],[uint16Field],[int32Field],"
					L"[uint32Field],[int64Field],[uint64Field],[floatField],[doubleField],[doubleField],[nullField])"
					L"VALUES (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \"Nothing\", 11)");

				auto updateSql = String(L"UPDATE [test] SET [field1] = 23 WHERE Id = 2");
				auto update = db.CompileStatement(updateSql);

				StatementDecorators::ExtendableUpdate statement(&db, std::move(update), updateSql);
			
				statement.StoreProperty(L"boolField", True);
				statement.StoreProperty(L"int8Field", -16);
				statement.StoreProperty(L"uint8Field", 16);
				statement.StoreProperty(L"int16Field", -512);
				statement.StoreProperty(L"uint16Field", 512);
				statement.StoreProperty(L"int32Field", -131072);
				statement.StoreProperty(L"uint32Field", 131072);
				statement.StoreProperty(L"int64Field", -8589934592);
				statement.StoreProperty(L"uint64Field", 8589934592);
				statement.StoreProperty(L"floatField", 100.61);
				statement.StoreProperty(L"doubleField", 10000.61);
				statement.StoreProperty(L"stringField", String(L"Some"));
				statement.StoreNull(L"nullField");
				statement.Execute();

				auto select = db.ExecuteQuery(
					L"SELECT * FROM [test] WHERE Id = 2");

				auto field1 = select->LoadProperty(L"field1", 0);
				Assert::AreEqual(23, field1);

				auto boolField = select->LoadProperty(L"boolField", False);
				Assert::AreEqual(True, boolField);

				auto int8Field = select->LoadProperty(L"int8Field", (Int8)0);
				Assert::IsTrue(-16 == int8Field);

				auto uint8Field = select->LoadProperty(L"uint8Field", (UInt8)0);
				Assert::IsTrue(16 == uint8Field);

				auto int16Field = select->LoadProperty(L"int16Field", (Int16)0);
				Assert::IsTrue(-512 == int16Field);

				auto uint16Field = select->LoadProperty(L"uint16Field", (UInt16)0);
				Assert::IsTrue(512 == uint16Field);

				auto int32Field = select->LoadProperty(L"int32Field", (Int32)0);
				Assert::IsTrue(-131072 == int32Field);

				auto uint32Field = select->LoadProperty(L"uint32Field", (UInt32)0);
				Assert::IsTrue(131072 == uint32Field);

				auto int64Field = select->LoadProperty(L"int64Field", (Int64)0);
				Assert::IsTrue(-8589934592 == int64Field);

				auto uint64Field = select->LoadProperty(L"uint64Field", (UInt64)0);
				Assert::IsTrue(8589934592 == uint64Field);

				auto floatField = select->LoadProperty(L"floatField", (Float)0);
				Assert::IsTrue(0 != floatField);

				auto doubleField = select->LoadProperty(L"doubleField", (Double)0);
				Assert::IsTrue(0 != doubleField);

				auto stringField = select->LoadProperty(L"stringField", Text(""));
				Assert::AreEqual(L"Some", stringField);

				auto nullField = select->LoadProperty(L"nullField", 0);
				Assert::IsTrue(NULL == nullField);
			}();
		}
	};
}
