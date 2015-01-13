#include "Statement.h"
#include "SqliteException.h"

#include <assert.h>
#include <string.h>
#include <tchar.h>

namespace WinToolsLib { namespace Data	{ namespace Sqlite
{
	using namespace Details;

	Statement::Statement(Database* db, sqlite3_stmt* stmt) :
		m_db(db),
		m_stmt(stmt),
		m_lastId(0),
		m_rowsModified(0),
		m_counter(0)
	{
	}

	Statement::~Statement()
	{
		Finalize();
	}

	Statement::Statement(Statement&& other)
	{
		MoveFrom(other);
	}

	Statement& Statement::operator=(Statement&& other)
	{
		Finalize();
		MoveFrom(other);

		return *this;
	}

	Void Statement::Finalize()
	{
		if (m_stmt != nullptr)
		{
			auto result = sqlite3_finalize(m_stmt);
			assert(SQLITE_OK == result);
			m_stmt = nullptr;
		}
	}

	Int64 Statement::Execute()
	{
		for (auto attempt = 0;;)
		{
			auto result = sqlite3_step(m_stmt);
			auto handle = sqlite3_db_handle(m_stmt);
			m_lastId = sqlite3_last_insert_rowid(handle);
			m_rowsModified = sqlite3_changes(handle);
			auto errmsg = sqlite3_errmsg(handle);

			sqlite3_reset(m_stmt);
			if (SQLITE_DONE == result)
			{
				return m_lastId;
			}

			if (SQLITE_BUSY == result && attempt++ < 3)
			{
				continue;
			}

			THROW_SQLITE_EXCEPTION2(result, errmsg);
		}
	}

	Void Statement::BeginStoreProperties()
	{
		m_counter++;
	}

	Void Statement::EndStoreProperties()
	{
		if (0 == --m_counter)
			Execute();
	}

	Void Statement::StoreProperty(const TChar* name, Bool value, BoolFormat format)
	{
		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int(m_stmt, column, value ? 1 : 0);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, Int8 value, IntFormat format)
	{
		if (IntFormat::Dec != format)
		{
			THROW_UNSUPPORTED_FORMAT_EXCEPTION();
		}

		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, UInt8 value, IntFormat format)
	{
		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, Int16 value, IntFormat format)
	{
		if (IntFormat::Dec != format)
		{
			THROW_UNSUPPORTED_FORMAT_EXCEPTION();
		}

		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, UInt16 value, IntFormat format)
	{
		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, Int32 value, IntFormat format)
	{
		if (IntFormat::Dec != format)
		{
			THROW_UNSUPPORTED_FORMAT_EXCEPTION();
		}

		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, UInt32 value, IntFormat format)
	{
		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, Int64 value, IntFormat format)
	{
		if (IntFormat::Dec != format)
		{
			THROW_UNSUPPORTED_FORMAT_EXCEPTION();
		}

		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int64(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, UInt64 value, IntFormat format)
	{
		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_int64(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, Float value, FloatFormat format)
	{
		if (FloatFormat::Default != format)
		{
			THROW_UNSUPPORTED_FORMAT_EXCEPTION();
		}

		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_double(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, Double value, FloatFormat format)
	{
		if (FloatFormat::Default != format)
		{
			THROW_UNSUPPORTED_FORMAT_EXCEPTION();
		}

		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_double(m_stmt, column, value);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, const String& value)
	{
		const auto column = FindColumn(name);
				
		const TChar* data = nullptr;
		auto size = value.GetLength() * sizeof(TChar);
		TChar emptyString[] = Text("");
		if (size)
		{
			data = value.GetBuffer();
		}
		else
		{
			data = emptyString;
			size = sizeof(emptyString);
		}

		auto result = sqlite3_bind_text16(m_stmt, column, data, (Int32)size, SQLITE_TRANSIENT);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Void Statement::StoreProperty(const TChar* name, const Buffer& value)
	{
		if (value.GetSize() == 0)
			return;

		const auto column = FindColumn(name);

		auto result = sqlite3_bind_blob(m_stmt, column, value.GetBuffer(), (Int32)value.GetSize(), SQLITE_TRANSIENT);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	IStorePropertyPtr Statement::StoreChild(const TChar* name)
	{
		return IStorePropertyPtr();
	}

	Void Statement::StoreNull(const TChar* name)
	{
		const auto column = FindColumn(name);
				
		auto result = sqlite3_bind_null(m_stmt, column);
		if (SQLITE_OK != result)
		{
			THROW_SQLITE_EXCEPTION(result);
		}
	}

	Int32 Statement::FindColumn(const TChar* name) const
	{
		const auto param = String::FormatS(Text("@%s"), name);
		const auto column = sqlite3_bind_parameter_index(m_stmt, StringA::Convert(param));

		if (0 == column)
		{
			THROW_COLUMN_NOT_FOUND_EXCEPTION(name);
		}
		return column;
	}

	Void Statement::MoveFrom(Statement& other)
	{
		if (this != &other)
		{
			m_db = other.m_db;
			m_stmt = other.m_stmt;
			m_lastId = other.m_lastId;
			m_counter = other.m_counter;
			m_rowsModified = other.m_rowsModified;

			other.m_db = nullptr;
			other.m_stmt = nullptr;
		}
	}
} } }