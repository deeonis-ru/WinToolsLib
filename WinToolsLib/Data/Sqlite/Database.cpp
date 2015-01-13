#include "Database.h"
#include "SqliteException.h"

#include "..\..\ScopeGuard.h"

#include <assert.h>

#define THROW_DATABASE_NOT_OPEN_EXCEPTION() \
	throw DatabaseNotOpen(__FUNCTION__, __LINE__, __FILE__)

#define THROW_DATABASE_NOT_EQUAL_EXCEPTION() \
	throw DatabaseNotEqual(__FUNCTION__, __LINE__, __FILE__)

namespace WinToolsLib
{
	namespace Data
	{
		namespace Sqlite
		{
			using namespace Details;

			Database::Database(sqlite3* db) :
				m_db(db)
			{
			}

			Database::Database() :
				m_db(nullptr)
			{
			}

			Database::~Database()
			{
				Close();
			}

			Database::Database(Database&& other) :
				m_db(other.m_db)
			{
				other.m_db = nullptr;
			}

			Database& Database::operator=(Database&& other)
			{
				Close();
				m_db = other.m_db;
				other.m_db = nullptr;
				return *this;
			}

			Database Database::Open(const WChar* path)
			{
				sqlite3* db = nullptr;

				auto result = sqlite3_open16(path, &db);
				if (SQLITE_OK != result)
				{
					THROW_SQLITE_EXCEPTION(result);
				}

				sqlite3_busy_timeout(db, 3333);

				return Database(db);
			}

			Void Database::Close()
			{
				if (m_db != nullptr)
				{
					auto result = sqlite3_close(m_db);
					assert(SQLITE_OK == result);
					m_db = nullptr;
				}
			}

			StatementPtr Database::CompileStatement(const WChar* sql)
			{
				if (m_db == nullptr)
				{
					THROW_DATABASE_NOT_OPEN_EXCEPTION();
				}

				auto stmt = Compile(sql);
				return StatementPtr(new Statement(this, stmt));
			}

			Void Database::CompileStatement(const WChar* sql, Statement* statement)
			{
				if (m_db == nullptr)
				{
					THROW_DATABASE_NOT_OPEN_EXCEPTION();
				}

				if (this != statement->m_db)
				{
					THROW_DATABASE_NOT_EQUAL_EXCEPTION();
				}

				statement->Finalize();
				statement->m_stmt = Compile(sql);
			}

			QueryPtr Database::ExecuteQuery(const WChar* sql)
			{
				if (m_db == nullptr)
				{
					THROW_DATABASE_NOT_OPEN_EXCEPTION();
				}

				auto stmt = Compile(sql);
				auto cleanup = OnExit([&stmt]()
				{
					if (stmt != nullptr)
					{
						sqlite3_finalize(stmt);
					}
				});

				auto result = sqlite3_step(stmt);
				if (SQLITE_DONE == result) // No rows
				{
					cleanup.Dismiss();
					return QueryPtr(new Query(this, stmt));
				}
				else if (SQLITE_ROW == result) // At least one row
				{
					cleanup.Dismiss();
					return QueryPtr(new Query(this, stmt));
				}
				
				THROW_SQLITE_EXCEPTION(result);
			}

			SizeT Database::ExecuteScalar(const WChar* sql)
			{
				if (m_db == nullptr)
				{
					THROW_DATABASE_NOT_OPEN_EXCEPTION();
				}

				auto stmt = Compile(sql);
				auto cleanup = OnExit([&stmt]()
				{
					if (stmt != nullptr)
					{
						sqlite3_finalize(stmt);
					}
				});

				auto result = sqlite3_step(stmt);			
				if (SQLITE_ROW == result)
				{
#ifdef _WIN64
					return (SizeT)sqlite3_column_int64(stmt, 0);
#else
					return (SizeT)sqlite3_column_int(stmt, 0);
#endif
				}

				THROW_SQLITE_EXCEPTION(result);
			}

			Void Database::Execute(const WChar* sql)
			{
				if (m_db == nullptr)
				{
					THROW_DATABASE_NOT_OPEN_EXCEPTION();
				}

				auto stmt = Compile(sql);
				auto cleanup = OnExit([&stmt]()
				{
					if (stmt != nullptr)
					{
						sqlite3_finalize(stmt);
					}
				});

				auto result = sqlite3_step(stmt);
				if (SQLITE_DONE != result &&
					SQLITE_ROW != result)
				{
					THROW_SQLITE_EXCEPTION(result);
				}
			}

			sqlite3_stmt* Database::Compile(const WChar* sql)
			{
				sqlite3_stmt* stmt = nullptr;

				const auto length = StringW::GetLength(sql);
				const auto bytes = length * sizeof(WChar);

				const auto result = sqlite3_prepare16_v2(m_db, sql, (Int32)bytes, &stmt, nullptr);
				if (SQLITE_OK != result)
				{
					if (SQLITE_ERROR == result)
					{
						const auto errmsg = sqlite3_errmsg(m_db);
						THROW_SQLITE_EXCEPTION2(result, errmsg);
					}
					else
					{
						THROW_SQLITE_EXCEPTION(result);
					}
				}

				return stmt;
			}
		}
	}
}