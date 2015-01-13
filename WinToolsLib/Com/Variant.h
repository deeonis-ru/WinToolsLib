#pragma once
#include "..\Types.h"
#include "..\String.h"
#include "..\Exception.h"

#include <Windows.h>

#include <vector>

namespace WinToolsLib { namespace Com
{
	class Variant
	{
	public:
		Variant(VARIANT* pvar = nullptr);
		Variant(Variant&& other);
		Variant(const Variant& other);
		~Variant();

		Void Clear();

		operator const VARIANT&() const;

		VARIANT* operator&();
		VARIANT* operator->();

		Variant& operator=(Variant&& other);
		Variant& operator=(const Variant& other);

		typedef std::vector<StringW> StringArray;

		StringArray ToStringArray() const;
		static Variant FromStringArray(const StringArray& stringArray);

	protected:
		Void MoveFrom(Variant& other);
		Void CopyFrom(const Variant& other);

	private:
		VARIANT m_var;
	};

	inline Variant::operator const VARIANT&() const
	{
		return m_var;
	}

	inline VARIANT* Variant::operator&()
	{
		return &m_var;
	}

	inline VARIANT* Variant::operator->()
	{
		return &m_var;
	}

	class VariantWrongTypeException : public Exception
	{
	public:
		VariantWrongTypeException(PCChar function, UInt32 line, PCChar file, VARTYPE expectedType) :
			Exception(function, line, file, CreateErrorMessage(expectedType)),
			m_expectedType(expectedType)
		{
		}

	protected:
		static String CreateErrorMessage(VARTYPE expectedType)
		{
			return String::FormatS(Text("Variant has wrong type, was expected: %hu"), expectedType);
		}

	private:
		VARTYPE m_expectedType;
	};

} }

