#pragma once
#include "..\..\Types.h"
#include "Database.h"

namespace WinToolsLib { namespace Data { namespace Sqlite
{
	class Transaction
	{
	public:
		Transaction(Database* db);
		~Transaction();

		Transaction(Transaction&& other);
		Transaction& operator=(Transaction&& other);

		Void Commit();
		Void Rollback();

	private:
		Void MoveFrom(Transaction& other);

	private:
		Database* m_db;
		Bool m_isFinished;
	};
} } }