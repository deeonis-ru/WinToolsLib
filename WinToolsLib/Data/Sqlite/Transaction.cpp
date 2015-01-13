#include "Transaction.h"

namespace WinToolsLib { namespace Data { namespace Sqlite
{
	using namespace Details;

	Transaction::Transaction(Database* db) :
		m_db(db),
		m_isFinished(False)
	{
		m_db->Execute(L"BEGIN;");
	}

	Transaction::~Transaction()
	{
		Rollback();
	}

	Transaction::Transaction(Transaction&& other)
	{
		MoveFrom(other);
	}

	Transaction& Transaction::operator=(Transaction&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Void Transaction::Commit()
	{
		if (!m_isFinished)
		{
			m_db->Execute(L"COMMIT;");
			m_isFinished = True;
		}
	}

	Void Transaction::Rollback()
	{
		if (!m_isFinished)
		{
			m_db->Execute(L"ROLLBACK;");
			m_isFinished = True;
		}
	}

	Void Transaction::MoveFrom(Transaction& other)
	{
		if (&other != this)
		{
			m_db = other.m_db;
			m_isFinished = std::move(other.m_isFinished);

			other.m_db = nullptr;
			other.m_isFinished = True;
		}
	}
} } }