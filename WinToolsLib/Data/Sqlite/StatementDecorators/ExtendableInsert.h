#pragma once
#include "ExtendableBase.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite { namespace StatementDecorators
{
	using namespace Serialization;

	class ExtendableInsert : public ExtendableBase
	{
		ExtendableInsert(const ExtendableInsert& other);            // Non-copyable
		ExtendableInsert& operator=(const ExtendableInsert& other); // Non-copyable
			
	public:
		ExtendableInsert(Database* db, StatementPtr&& statement, const TChar* sql);
		~ExtendableInsert();

		ExtendableInsert(ExtendableInsert&& other);
		ExtendableInsert& operator=(ExtendableInsert&& other);

	protected:
		Void ModifySql(const TChar* name, const TChar* type) override;

	};
} } } }

