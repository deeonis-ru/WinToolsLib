#include "ExtendableBase.h"

#include <regex>
#include <assert.h>

#include "..\Database.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite { namespace StatementDecorators
{
	using namespace Details;

	ExtendableBase::ExtendableBase(Database* db, StatementPtr&& statement, const TChar* sql) :
		m_statement(std::move(statement)),
		m_sql(sql),
		m_db(db)
	{
	}

	ExtendableBase::~ExtendableBase()
	{
	}

	ExtendableBase::ExtendableBase(ExtendableBase&& other)
	{
		MoveFrom(other);
	}

	ExtendableBase& ExtendableBase::operator=(ExtendableBase&& other)
	{
		MoveFrom(other);
				
		return *this;
	}

	Void ExtendableBase::StoreProperty(const TChar* name, const String& value)
	{
		try
		{
			m_statement->StoreProperty(name, value);
		}
		catch (ColumnNotFoundException&)
		{
			RecreateStatement(name, Text("TEXT"));
			m_statement->StoreProperty(name, value);
		}

		TChar emptyStr[] = Text("");
		auto empty = value.IsEmpty();
		Buffer data( empty ? (PCByte)emptyStr : (PCByte)value.GetBuffer(), empty ? sizeof(emptyStr) : (value.GetLength() + 1) * sizeof(TChar));
		auto prop = std::make_tuple(name, std::move(data), typeid(value.GetBuffer()).name(), -1);
		m_propertyList.push_back(std::move(prop));
	}

	Void ExtendableBase::StoreProperty(const TChar* name, const Buffer& value)
	{
		assert(False);
		//
		// TODO: implement
		//
	}

	Void ExtendableBase::StoreNull(const TChar* name)
	{
		try
		{
			m_statement->StoreNull(name);
		}
		catch (ColumnNotFoundException&)
		{
			RecreateStatement(name, Text("NULL"));
			m_statement->StoreNull(name);
		}

		Buffer data;
		auto prop = std::make_tuple(name, std::move(data), "", -1);
		m_propertyList.push_back(std::move(prop));
	}

	Void ExtendableBase::MoveFrom(ExtendableBase& other)
	{
		if (this != &other)
		{
			m_statement = std::move(other.m_statement);
			m_sql = std::move(other.m_sql);
			m_db = std::move(other.m_db);
		}
	}

	String ExtendableBase::SearchByRegExp(const TChar* regExp) const
	{
		#ifdef _UNICODE
		std::wregex rgx(regExp);
		std::wcmatch match;
#elif
		std::regex rgx(regExp);
		std::cmatch match;
#endif

		if (std::regex_search(m_sql.GetBuffer(), match, rgx) && match.size() > 0)
			return std::move(String(match[0].str().c_str()));
		else
			return std::move(String());
	}

	Void ExtendableBase::ReplaceInSql(const String& replaceableStr, const TChar* format, const TChar* additionalStr)
	{
		if (!replaceableStr.IsEmpty())
		{
			String oldStr(replaceableStr.GetBuffer(), replaceableStr.GetSize() - 1);
			auto newStr = String::FormatS(format, oldStr.GetBuffer(), additionalStr);
			m_sql.Replace(oldStr, newStr);
		}
		else
		{
			THROW_COLUMN_NOT_FOUND_EXCEPTION(additionalStr);
		}
	}

	Void ExtendableBase::StorePropertyList()
	{
		for (const auto& prop : m_propertyList)
		{
			const auto& name = std::get<0>(prop);
			const auto& value = std::get<1>(prop);
			const auto& valueType = std::get<2>(prop);
			auto format = std::get<3>(prop);

			if (typeid(Bool).name() == valueType)
				m_statement->StoreProperty(name, *(Bool*)value.GetBuffer(), (BoolFormat)format);
			else if (typeid(Int8).name() == valueType)
				m_statement->StoreProperty(name, *(Int8*)value.GetBuffer(), (IntFormat)format);
			else if (typeid(UInt8).name() == valueType)
				m_statement->StoreProperty(name, *(UInt8*)value.GetBuffer(), (IntFormat)format);
			else if (typeid(Int16).name() == valueType)
				m_statement->StoreProperty(name, *(Int16*)value.GetBuffer(), (IntFormat)format);
			else if (typeid(UInt16).name() == valueType)
				m_statement->StoreProperty(name, *(UInt16*)value.GetBuffer(), (IntFormat)format);
			else if (typeid(Int32).name() == valueType)
				m_statement->StoreProperty(name, *(Int32*)value.GetBuffer(), (IntFormat)format);
			else if (typeid(UInt32).name() == valueType)
				m_statement->StoreProperty(name, *(UInt32*)value.GetBuffer(), (IntFormat)format);
			else if (typeid(Int64).name() == valueType)
				m_statement->StoreProperty(name, *(Int64*)value.GetBuffer(), (IntFormat)format);
			else if (typeid(UInt64).name() == valueType)
				m_statement->StoreProperty(name, *(UInt64*)value.GetBuffer(), (IntFormat)format);
			else if (typeid(Float).name() == valueType)
				m_statement->StoreProperty(name, *(Float*)value.GetBuffer(), (FloatFormat)format);
			else if (typeid(Double).name() == valueType)
				m_statement->StoreProperty(name, *(Double*)value.GetBuffer(), (FloatFormat)format);
			else if (typeid(const TChar*).name() == valueType)
				m_statement->StoreProperty(name, String((TChar*)(value.GetBuffer())));
			else if ("" == valueType)
				m_statement->StoreNull(name);
		}
	}

	Void ExtendableBase::RecreateStatement(const TChar* name, const TChar* type)
	{
		ModifySql(name, type);

		m_db->CompileStatement(m_sql, static_cast<Statement*>(m_statement.get()));
		StorePropertyList();
	}
} } } }