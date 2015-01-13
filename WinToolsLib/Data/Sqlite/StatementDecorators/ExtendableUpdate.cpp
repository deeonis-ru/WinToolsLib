#include "ExtendableUpdate.h"

#include "..\Database.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite { namespace StatementDecorators
{
	using namespace Details;

	ExtendableUpdate::ExtendableUpdate(Database* db, StatementPtr&& statement, const TChar* sql) :
		ExtendableBase(db, std::move(statement), sql)
	{
	}

	ExtendableUpdate::~ExtendableUpdate()
	{
	}

	ExtendableUpdate::ExtendableUpdate(ExtendableUpdate&& other) :
		ExtendableBase(std::move(other))
	{
	}

	ExtendableUpdate& ExtendableUpdate::operator=(ExtendableUpdate&& other)
	{
		MoveFrom(other);
				
		return *this;
	}

	Void ExtendableUpdate::ModifySql(const TChar* name, const TChar* type)
	{
		auto fieldValue = String::FormatS(Text("[%s] = @%s, "), name, name);
		auto set = SearchByRegExp(Text("SET\\s{1,}"));
		ReplaceInSql(set, Text("%s %s"), fieldValue);
	}
} } } }