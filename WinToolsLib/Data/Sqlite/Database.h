#pragma once
#include "..\..\Types.h"

#include "Query.h"
#include "Statement.h"

namespace WinToolsLib
{
	namespace Data
	{
		namespace Sqlite
		{
			namespace Details
			{
				#include "Details\sqlite3.h"
			}
			
			class Database
			{
				Database(const Database& other);            // Non-copyable
				Database& operator=(const Database& other); // Non-copyable

				Database(Details::sqlite3* db);
			
			public:
				Database();
				~Database();
				
				Database(Database&& other);
				Database& operator=(Database&& other);

				static Database Open(const WChar* path);
				Void Close();

				StatementPtr CompileStatement(const WChar* sql);
				Void CompileStatement(const WChar* sql, Statement* statement);
				QueryPtr ExecuteQuery(const WChar* sql);
				SizeT ExecuteScalar(const WChar* sql);
				Void Execute(const WChar* sql);

			private:
				Details::sqlite3_stmt* Compile(const WChar* sql);

			private:
				Details::sqlite3* m_db;
			};

			class DatabaseNotOpen : public WinToolsLib::Exception
			{
			public:
				DatabaseNotOpen(PCChar function, UInt32 line, PCChar file) :
					Exception(function, line, file, Text("Database not open"))
				{
				}
			};

			class DatabaseNotEqual : public WinToolsLib::Exception
			{
			public:
				DatabaseNotEqual(PCChar function, UInt32 line, PCChar file) :
					Exception(function, line, file, Text("Database not equal"))
				{
				}
			};
		}
	}
}

