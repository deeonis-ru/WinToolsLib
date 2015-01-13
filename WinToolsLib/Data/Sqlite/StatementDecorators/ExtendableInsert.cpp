#include "ExtendableInsert.h"

#include "..\Database.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite { namespace StatementDecorators
{
	using namespace Details;

	ExtendableInsert::ExtendableInsert(Database* db, StatementPtr&& statement, const TChar* sql) :
		ExtendableBase(db, std::move(statement), sql)
	{
	}

	ExtendableInsert::~ExtendableInsert()
	{
	}

	ExtendableInsert::ExtendableInsert(ExtendableInsert&& other) :
		ExtendableBase(std::move(other))
	{
	}

	ExtendableInsert& ExtendableInsert::operator=(ExtendableInsert&& other)
	{
		MoveFrom(other);
				
		return *this;
	}

	Void ExtendableInsert::ModifySql(const TChar* name, const TChar* type)
	{
		auto field = SearchByRegExp(Text("\\[\\w{1,100}\\]\\s{0,}\\)"));
		ReplaceInSql(field, Text("%s, [%s]"), name);
		
		auto value = SearchByRegExp(Text("VALUES\\s{0,}\\(.*\\)"));
		ReplaceInSql(value, Text("%s, @%s"), name);

		auto table = SearchByRegExp(Text("\\[\\w{1,100}\\]"));
		String tableName(table.GetBuffer() + 1, table.GetSize() - 2);
		StringW sql = StringW::FormatS(L"ALTER TABLE [%s] ADD [%s] %s;", tableName.GetBuffer(), name, type);
		try
		{
			m_db->Execute(sql);
		}
		catch (SqliteException& ex)
		{
			if (ex.GetErrorCode() != SQLITE_ERROR)
				throw;
		}
	}
} } } }