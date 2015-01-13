#pragma once
#include "..\..\Types.h"
#include "Statement.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite
{
	template <class T>
	class TemplateStatement : public Statement
	{
	public:
		TemplateStatement(Statement&& base);

		TemplateStatement(TemplateStatement&& other);
		TemplateStatement& operator=(TemplateStatement&& other);

		virtual IStorePropertyPtr StoreChild(const TChar* name) override;

	private:
		static const StringW m_sql;
	};

	template <class T>
	TemplateStatement<T>::TemplateStatement(Statement&& base) :
		Statement(std::move(base))
	{}

	template <class T>
	TemplateStatement<T>::TemplateStatement(TemplateStatement<T>&& other) :
		Statement(std::move(other))
	{}

	template <class T>
	TemplateStatement<T>& TemplateStatement<T>::operator=(TemplateStatement<T>&& other)
	{
		__super::operator=(std::move(other));
		return *this;
	}

	template <class T>
	IStorePropertyPtr TemplateStatement<T>::StoreChild(const TChar* name)
	{
		if (m_sql.IsEmpty())
			return IStorePropertyPtr();

		auto sql = String::FormatS(m_sql.GetBuffer(), name, m_lastId);
	
		auto statement = m_db->CompileStatement(sql);
		auto rawPtr = static_cast<Statement*>(statement.get());
		return IStorePropertyPtr(new T(std::move(*rawPtr)));
	}

#define DEFINE_STATEMENT_CLASS(classname) \
	class classname : public Statement \
	{ \
	public: \
		classname(Statement&& base); \
		classname(classname&& other); \
		classname& operator=(classname&& other); \
	}; \
	inline classname::classname(Statement&& base) : \
		Statement(std::move(base)) \
	{} \
	inline classname::classname(classname&& other) : \
		Statement(std::move(other)) \
	{} \
	inline classname& classname::operator=(classname&& other) \
	{ \
		MoveFrom(other); \
		return *this; \
	}

#define BEGIN_TEMPLATE_STATEMENT_SQL namespace WinToolsLib { namespace Data { namespace Sqlite {
#define END_TEMPLATE_STATEMENT_SQL } } }
} } }