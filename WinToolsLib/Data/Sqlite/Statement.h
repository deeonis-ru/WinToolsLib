#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Exception.h"
#include "IStatement.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite
{
	class Database;

	namespace Details
	{
		#include "Details\sqlite3.h"
	}

	using namespace Serialization;

	class Statement : public IStatement
	{
		Statement(const Statement& other);            // Non-copyable
		Statement& operator=(const Statement& other); // Non-copyable

		Statement(Database* db, Details::sqlite3_stmt* stmt);
			
	public:
		virtual ~Statement();

		Statement(Statement&& other);
		Statement& operator=(Statement&& other);

		Void Finalize() override;
		Int64 Execute() override;

		Void BeginStoreProperties() override;
		Void EndStoreProperties() override;

		Void StoreProperty(const TChar* name, Bool value, BoolFormat format = BoolFormat::TrueFalse) override;

		Void StoreProperty(const TChar* name, Int8 value, IntFormat format = IntFormat::Dec) override;
		Void StoreProperty(const TChar* name, UInt8 value, IntFormat format = IntFormat::Dec) override;

		Void StoreProperty(const TChar* name, Int16 value, IntFormat format = IntFormat::Dec) override;
		Void StoreProperty(const TChar* name, UInt16 value, IntFormat format = IntFormat::Dec) override;

		Void StoreProperty(const TChar* name, Int32 value, IntFormat format = IntFormat::Dec) override;
		Void StoreProperty(const TChar* name, UInt32 value, IntFormat format = IntFormat::Dec) override;

		Void StoreProperty(const TChar* name, Int64 value, IntFormat format = IntFormat::Dec) override;
		Void StoreProperty(const TChar* name, UInt64 value, IntFormat format = IntFormat::Dec) override;

		Void StoreProperty(const TChar* name, Float value, FloatFormat format = FloatFormat::Default) override;
		Void StoreProperty(const TChar* name, Double value, FloatFormat format = FloatFormat::Default) override;

		Void StoreProperty(const TChar* name, const String& value) override;

		Void StoreProperty(const TChar* name, const Buffer& value) override;

		IStorePropertyPtr StoreChild(const TChar* name) override;

		Void StoreNull(const TChar* name) override;

		Int64 GetLastId() const override;
		Int32 GetRowsModified() const override;

	protected:
		Int32 FindColumn(const TChar* name) const;

		Void MoveFrom(Statement& other);

	protected:
		Database*              m_db;
		Details::sqlite3_stmt* m_stmt;
		Int64 m_lastId;
		Int32 m_rowsModified;
		UInt32 m_counter;

		friend class Database;
	};

	inline Int64 Statement::GetLastId() const
	{
		return m_lastId;
	}

	inline Int32 Statement::GetRowsModified() const
	{
		return m_rowsModified;
	}

} } }

