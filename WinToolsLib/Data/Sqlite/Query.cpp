#include "Query.h"
#include "SqliteException.h"

#include <assert.h>
#include <string.h>
#include <tchar.h>

namespace WinToolsLib
{
	namespace Data
	{
		namespace Sqlite
		{
			using namespace Details;

			Query::Query(Database* db, sqlite3_stmt* stmt) :
				m_db(db),
				m_stmt(stmt)
			{
				auto count = sqlite3_column_count(stmt);
				for (auto i = 0; i < count; i++)
				{
					auto next = (const WChar*)sqlite3_column_name16(m_stmt, i);
					m_columns[next] = i;
				}

			}

			Query::~Query()
			{
				Finalize();
			}

			Query::Query(Query&& other)
			{
				MoveFrom(other);
			}

			Query& Query::operator=(Query&& other)
			{
				Finalize();
				MoveFrom(other);
				
				return *this;
			}

			Void Query::Finalize()
			{
				if (m_stmt != nullptr)
				{
					auto result = sqlite3_finalize(m_stmt);
					assert(SQLITE_OK == result);
					m_stmt = nullptr;
				}
			}

			Bool Query::NextRow()
			{
				auto result = sqlite3_step(m_stmt);
				return SQLITE_ROW == result;
			}

			Bool Query::LoadProperty(const TChar* name, Bool defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					auto column_value = sqlite3_column_int(m_stmt, column);
					res = !(column_value == 0);
				}

				return res;
			}

			Int8 Query::LoadProperty(const TChar* name, Int8 defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = sqlite3_column_int(m_stmt, column);
				}

				return res;
			}

			UInt8 Query::LoadProperty(const TChar* name, UInt8 defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = sqlite3_column_int(m_stmt, column);
				}

				return res;
			}

			Int16 Query::LoadProperty(const TChar* name, Int16 defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = sqlite3_column_int(m_stmt, column);
				}

				return res;
			}

			UInt16 Query::LoadProperty(const TChar* name, UInt16 defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = sqlite3_column_int(m_stmt, column);
				}

				return res;
			}

			Int32 Query::LoadProperty(const TChar* name, Int32 defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = sqlite3_column_int(m_stmt, column);
				}

				return res;
			}

			UInt32 Query::LoadProperty(const TChar* name, UInt32 defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = static_cast<UInt32>(sqlite3_column_int(m_stmt, column));
				}

				return res;
			}

			Int64 Query::LoadProperty(const TChar* name, Int64 defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = static_cast<Int64>(sqlite3_column_int64(m_stmt, column));
				}

				return res;
			}

			UInt64 Query::LoadProperty(const TChar* name, UInt64 defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = static_cast<UInt64>(sqlite3_column_int64(m_stmt, column));
				}

				return res;
			}

			Float Query::LoadProperty(const TChar* name, Float defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = static_cast<Float>(sqlite3_column_double(m_stmt, column));
				}

				return res;
			}

			Double Query::LoadProperty(const TChar* name, Double defaultValue) const
			{
				auto res = defaultValue;

				const auto column = FindColumn(name);
				if (!IsNull(column))
				{
					res = sqlite3_column_double(m_stmt, column);
				}

				return res;
			}

			String Query::LoadProperty(const TChar* name, const TChar* defaultValue) const
			{
				const auto column = FindColumn(name);
				if (IsNull(column))
				{
					return String(defaultValue);
				}

				return (const WChar*)sqlite3_column_text16(m_stmt, column);
			}

			Buffer Query::LoadProperty(const TChar* name, const Buffer& defaultValue) const
			{
				const auto column = FindColumn(name);
				if (IsNull(column))
				{
					return defaultValue;
				}

				auto size = sqlite3_column_bytes16(m_stmt, column);
				auto blob = sqlite3_column_blob(m_stmt, column);

				return Buffer(static_cast<PCByte>(blob), size);
			}

			ILoadPropertyPtr Query::LoadChild(const TChar* name) const
			{
				return ILoadPropertyPtr();
			}

			Void Query::LoadChildren(const TChar* name, LoadChildrenCallback callback) const
			{
			}

			Bool Query::IsNull(const TChar* name) const
			{
				const auto column = FindColumn(name);
				return SQLITE_NULL == sqlite3_column_type(m_stmt, column);
			}

			Bool Query::HasRows() const
			{
				return sqlite3_data_count(m_stmt) != 0;
			}

			Bool Query::IsNull(const Int32 column) const
			{
				return SQLITE_NULL == sqlite3_column_type(m_stmt, column);
			}

			Int32 Query::FindColumn(const TChar* name) const
			{
				auto found = m_columns.find(name);
				if (std::end(m_columns) != found)
				{
					return found->second;
				}
				THROW_COLUMN_NOT_FOUND_EXCEPTION(name);
			}

			Void Query::MoveFrom(Query& other)
			{
				if (this != &other)
				{
					m_db = other.m_db;
					m_columns = std::move(other.m_columns);
					m_stmt = other.m_stmt;

					other.m_db = nullptr;
					other.m_stmt = nullptr;
				}
			}
		}
	}
}