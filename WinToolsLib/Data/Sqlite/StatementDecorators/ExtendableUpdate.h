#pragma once
#include "ExtendableBase.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite { namespace StatementDecorators
{
	using namespace Serialization;

	class ExtendableUpdate : public ExtendableBase
	{
		ExtendableUpdate(const ExtendableUpdate& other);            // Non-copyable
		ExtendableUpdate& operator=(const ExtendableUpdate& other); // Non-copyable
			
	public:
		ExtendableUpdate(Database* db, StatementPtr&& statement, const TChar* sql);
		~ExtendableUpdate();

		ExtendableUpdate(ExtendableUpdate&& other);
		ExtendableUpdate& operator=(ExtendableUpdate&& other);

	protected:
		Void ModifySql(const TChar* name, const TChar* type) override;

	};
} } } }

