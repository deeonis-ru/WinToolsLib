#pragma once
#include "..\..\Types.h"
#include "Query.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite
{
	template <class T>
	class TemplateQuery : public Query
	{
	public:
		TemplateQuery(Query&& base);
		~TemplateQuery();

		TemplateQuery(TemplateQuery&& other);
		TemplateQuery& operator=(TemplateQuery&& other);

		Bool NextRow() override;

		Void LoadChildren(const TChar* name, LoadChildrenCallback callback) const override;
		ILoadPropertyPtr LoadChild(const TChar* name) const override;

	protected:
		Void MoveFrom(TemplateQuery& other);

	protected:
		Int64 m_id;

	private:
		static const StringW m_sql;
	};

	template <class T>
	TemplateQuery<T>::TemplateQuery(Query&& base) :
		Query(std::move(base)),
		m_id(0)
	{
		m_id = LoadProperty(Text("Id"), 0);
	}

	template <class T>
	TemplateQuery<T>::~TemplateQuery()
	{
	}

	template <class T>
	TemplateQuery<T>::TemplateQuery(TemplateQuery&& other) :
		Query(std::move(other)),
		m_id(std::move(other.m_id))
	{
	}

	template <class T>
	TemplateQuery<T>& TemplateQuery<T>::operator=(TemplateQuery&& other)
	{
		Query::operator=(std::move(other));
		MoveFrom(other);
		return *this;
	}

	template <class T>
	Bool TemplateQuery<T>::NextRow()
	{
		auto res = __super::NextRow();
		if (res)
		{
			m_id = LoadProperty(Text("Id"), 0);
		}
		return res;
	}

	template <class T>
	Void TemplateQuery<T>::LoadChildren(const TChar* name, LoadChildrenCallback callback) const
	{
		if (m_sql.IsEmpty())
			return;

		auto sql = StringW::FormatS(m_sql.GetBuffer(), name, m_id);

		auto query = m_db->ExecuteQuery(sql);
		if (!query->HasRows())
			return;

		auto queryChildren = std::unique_ptr<T>(new T(std::move(*query.release())));
		do
		{
			if (!callback(queryChildren.get()))
			{
				break;
			}
		}
		while (queryChildren->NextRow());
	}

	template <class T>
	ILoadPropertyPtr TemplateQuery<T>::LoadChild(const TChar* name) const
	{
		if (m_sql.IsEmpty())
			return ILoadPropertyPtr();

		auto sql = StringW::FormatS(m_sql.GetBuffer(), name, m_id);

		auto query = m_db->ExecuteQuery(sql);
		if (query->HasRows())
			return ILoadPropertyPtr(new T(std::move(*query.release())));
		else
			return ILoadPropertyPtr();
	}

	template <class T>
	Void TemplateQuery<T>::MoveFrom(TemplateQuery& other)
	{
		if (this != &other)
		{
			m_id = std::move(other.m_id);
		}
	}

#define DEFINE_QUERY_CLASS(classname) \
	class classname : public Query \
	{ \
	public: \
		classname(Query&& base); \
		classname(classname&& other); \
		classname& operator=(classname&& other); \
	}; \
	inline classname::classname(Query&& base) : \
		Query(std::move(base)) \
	{} \
	inline classname::classname(classname&& other) : \
		Query(std::move(other)) \
	{} \
	inline classname& classname::operator=(classname&& other) \
	{ \
		MoveFrom(other); \
		return *this; \
	}

#define BEGIN_TEMPLATE_QUERY_SQL namespace WinToolsLib { namespace Data { namespace Sqlite {
#define END_TEMPLATE_QUERY_SQL } } }
} } }