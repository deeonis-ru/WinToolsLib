#pragma once
#include "..\..\..\Types.h"
#include "..\..\..\Buffer.h"
#include "..\Statement.h"
#include "..\SqliteException.h"

namespace WinToolsLib { namespace Data	{ namespace Sqlite { namespace StatementDecorators
{
	using namespace Serialization;

	class ExtendableBase : public IStatement
	{
		ExtendableBase(const ExtendableBase& other);            // Non-copyable
		ExtendableBase& operator=(const ExtendableBase& other); // Non-copyable
			
	public:
		ExtendableBase(Database* db, StatementPtr&& statement, const TChar* sql);
		virtual ~ExtendableBase();

		ExtendableBase(ExtendableBase&& other);
		ExtendableBase& operator=(ExtendableBase&& other);

		Void Finalize() override;
		Int64 Execute() override;

		Void BeginStoreProperties() override;

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

		Void EndStoreProperties() override;

		IStorePropertyPtr StoreChild(const TChar* name) override;

		Void StoreNull(const TChar* name) override;

		Int64 GetLastId() const override;
		Int32 GetRowsModified() const override;

	protected:
		Void MoveFrom(ExtendableBase& other);
		String SearchByRegExp(const TChar* regExp) const;
		Void ReplaceInSql(const String& replaceableStr, const TChar* format, const TChar* additionalStr);
		virtual Void ModifySql(const TChar* name, const TChar* type) = 0;

	protected:
		String m_sql;
		Database* m_db;

	private:
		Void StorePropertyList();
		template <class Value, class Format>
		Void TryStore(const TChar* name, const TChar* type, Value value, Format format);
		Void RecreateStatement(const TChar* name, const TChar* type);

	private:
		typedef std::tuple<String, Buffer, StringA, Int32> StoredProperty;
		std::vector<StoredProperty> m_propertyList;
		StatementPtr m_statement;
	};

	inline Void ExtendableBase::Finalize()
	{
		m_statement->Finalize();
	}

	inline Int64 ExtendableBase::Execute()
	{
		m_propertyList.clear();
		return m_statement->Execute();
	}

	inline Void ExtendableBase::BeginStoreProperties()
	{
		m_statement->BeginStoreProperties();
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, Bool value, BoolFormat format)
	{
		TryStore(name, Text("BOOL"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, Int8 value, IntFormat format)
	{
		TryStore(name, Text("INTEGER"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, UInt8 value, IntFormat format)
	{
		TryStore(name, Text("INTEGER"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, Int16 value, IntFormat format)
	{
		TryStore(name, Text("INTEGER"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, UInt16 value, IntFormat format)
	{
		TryStore(name, Text("INTEGER"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, Int32 value, IntFormat format)
	{
		TryStore(name, Text("INTEGER"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, UInt32 value, IntFormat format)
	{
		TryStore(name, Text("INTEGER"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, Int64 value, IntFormat format)
	{
		TryStore(name, Text("INTEGER"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, UInt64 value, IntFormat format)
	{
		TryStore(name, Text("INTEGER"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, Float value, FloatFormat format)
	{
		TryStore(name, Text("REAL"), value, format);
	}

	inline Void ExtendableBase::StoreProperty(const TChar* name, Double value, FloatFormat format)
	{
		TryStore(name, Text("REAL"), value, format);
	}

	inline Void ExtendableBase::EndStoreProperties()
	{
		m_statement->EndStoreProperties();
	}

	inline IStorePropertyPtr ExtendableBase::StoreChild(const TChar* name)
	{
		return m_statement->StoreChild(name);
	}

	template <class Value, class Format>
	Void ExtendableBase::TryStore(const TChar* name, const TChar* type, Value value, Format format)
	{
		try
		{
			m_statement->StoreProperty(name, value, format);
		}
		catch (ColumnNotFoundException&)
		{
			RecreateStatement(name, type);
			m_statement->StoreProperty(name, value, format);
		}

		Buffer data((PCByte)&value, sizeof(value));
		auto prop = std::make_tuple(name, std::move(data), typeid(value).name(), (Int32)format);
		m_propertyList.push_back(std::move(prop));
	}

	inline Int64 ExtendableBase::GetLastId() const
	{
		return m_statement->GetLastId();
	}

	inline Int32 ExtendableBase::GetRowsModified() const
	{
		return m_statement->GetRowsModified();
	}

} } } }

