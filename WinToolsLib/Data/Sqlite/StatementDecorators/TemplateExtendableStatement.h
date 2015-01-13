#pragma once
#include "..\..\..\Types.h"
#include "..\Statement.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite { namespace StatementDecorators
{
	template <class T1, class T2>
	class TemplateExtendableStatement : public Statement
	{
	public:
		TemplateExtendableStatement(Statement&& base);

		TemplateExtendableStatement(TemplateExtendableStatement&& other);
		TemplateExtendableStatement& operator=(TemplateExtendableStatement&& other);

		virtual IStorePropertyPtr StoreChild(const TChar* name) override;

	private:
		static const StringW m_sql;
	};

	template <class T1, class T2>
	TemplateExtendableStatement<T1, T2>::TemplateExtendableStatement(Statement&& base) :
		Statement(std::move(base))
	{}

	template <class T1, class T2>
	TemplateExtendableStatement<T1, T2>::TemplateExtendableStatement(TemplateExtendableStatement<T1, T2>&& other) :
		Statement(std::move(other))
	{}

	template <class T1, class T2>
	TemplateExtendableStatement<T1, T2>& TemplateExtendableStatement<T1, T2>::operator=(TemplateExtendableStatement<T1, T2>&& other)
	{
		__super::operator=(std::move(other));
		return *this;
	}

	template <class T1, class T2>
	IStorePropertyPtr TemplateExtendableStatement<T1, T2>::StoreChild(const TChar* name)
	{
		if (m_sql.IsEmpty())
			return IStorePropertyPtr();

		auto sql = String::FormatS(m_sql.GetBuffer(), name, m_lastId);
	
		auto statement = m_db->CompileStatement(sql);
		auto rawPtr = static_cast<Statement*>(statement.get());
		auto statementPtr = StatementPtr(new T1(std::move(*rawPtr)));

		return IStorePropertyPtr(new T2(m_db, std::move(statementPtr), sql));
	}

#define BEGIN_TEMPLATE_EXTENDABLE_STATEMENT_SQL namespace WinToolsLib { namespace Data { namespace Sqlite { namespace StatementDecorators {
#define END_TEMPLATE_EXTENDABLE_STATEMENT_SQL } } } }
} } } }