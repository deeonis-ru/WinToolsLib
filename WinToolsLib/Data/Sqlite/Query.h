#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Serialization\ISerializer.h"

#include <map>
#include <memory>

namespace WinToolsLib { namespace Data	{ namespace Sqlite
{
	class Database;

	namespace Details
	{
		#include "Details\sqlite3.h"
	}

	using namespace Serialization;

	class Query : public ILoadProperty
	{
		Query(const Query& other);            // Non-copyable
		Query& operator=(const Query& other); // Non-copyable

		Query(Database* db, Details::sqlite3_stmt* stmt);

	public:
		virtual ~Query();

		Query(Query&& other);
		Query& operator=(Query&& other);

		Void Finalize();
		virtual Bool NextRow();

		virtual Bool LoadProperty(const TChar* name, Bool defaultValue) const override;

		virtual Int8 LoadProperty(const TChar* name, Int8 defaultValue) const override;
		virtual UInt8 LoadProperty(const TChar* name, UInt8 defaultValue) const override;

		virtual Int16 LoadProperty(const TChar* name, Int16 defaultValue) const override;
		virtual UInt16 LoadProperty(const TChar* name, UInt16 defaultValue) const override;

		virtual Int32 LoadProperty(const TChar* name, Int32 defaultValue) const override;
		virtual UInt32 LoadProperty(const TChar* name, UInt32 defaultValue) const override;

		virtual Int64 LoadProperty(const TChar* name, Int64 defaultValue) const override;
		virtual UInt64 LoadProperty(const TChar* name, UInt64 defaultValue) const override;

		virtual Float LoadProperty(const TChar* name, Float defaultValue) const override;
		virtual Double LoadProperty(const TChar* name, Double defaultValue) const override;

		virtual String LoadProperty(const TChar* name, const TChar* defaultValue) const override;

		virtual Buffer LoadProperty(const TChar* name, const Buffer& defaultValue) const override;

		virtual ILoadPropertyPtr LoadChild(const TChar* name) const override;
		virtual Void LoadChildren(const TChar* name, LoadChildrenCallback callback) const override;

		Bool IsNull(const TChar* name) const;
		Bool HasRows() const;

	protected:
		Bool IsNull(const Int32 column) const;
		Int32 FindColumn(const TChar* name) const;

		Void MoveFrom(Query& other);

	protected:
		Database*               m_db;
		Details::sqlite3_stmt*  m_stmt;
		std::map<String, Int32> m_columns;

		friend class Database;
	};

	typedef std::unique_ptr<Query> QueryPtr;
} } }

