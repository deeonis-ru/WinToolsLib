#include "Variant.h"
#include "BStr.h"
#include "SafeArray.h"

#include <OleAuto.h>
#pragma comment(lib, "OleAut32.lib")

#include <type_traits>

#define THROW_VARIANT_WRONG_TYPE_EXCEPTION(expected) \
	throw VariantWrongTypeException(__FUNCTION__, __LINE__, __FILE__, expected)

namespace WinToolsLib { namespace Com
{
	Variant::Variant(VARIANT* pvar)
	{
		::VariantInit(&m_var);
		if (pvar)
		{
			::VariantCopy(&m_var, pvar);
		}
	}

	Variant::Variant(Variant&& other)
	{
		::VariantInit(&m_var);
		MoveFrom(other);
	}

	Variant::Variant(const Variant& other)
	{
		::VariantInit(&m_var);
		CopyFrom(other);
	}

	Variant::~Variant()
	{
		::VariantClear(&m_var);
	}

	Void Variant::Clear()
	{
		::VariantClear(&m_var);
	}

	Variant& Variant::operator=(Variant&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Variant& Variant::operator=(const Variant& other)
	{
		CopyFrom(other);
		return *this;
	}

	Variant::StringArray Variant::ToStringArray() const
	{
		if (VT_NULL == m_var.vt ||
			VT_EMPTY == m_var.vt)
		{
			return StringArray();
		}

		if ((VT_ARRAY | VT_BSTR) != m_var.vt)
		{
			THROW_VARIANT_WRONG_TYPE_EXCEPTION((VT_ARRAY | VT_BSTR));
		}

		StringArray stringArray;
		SafeArray safeArray(m_var.parray);
		
		try
		{
			const auto size = (Int32)safeArray.GetSize();
			for (auto i = 0; i < size; i++)
			{
				BStr bstr;
				safeArray.GetElement(i, &bstr);
				stringArray.push_back(bstr.GetBuffer());
			}
		}
		catch (...)
		{
			// Detach m_var.parray to prevent its destruction
			safeArray.Detach();
			throw;
		}

		safeArray.Detach();
		return stringArray;
	}

	Variant Variant::FromStringArray(const StringArray& stringArray)
	{
		auto safeArray = SafeArray::Create(
			VT_BSTR, (UInt32)stringArray.size());

		auto index = 0;
		for (const auto& string : stringArray)
		{
			BStr bstr(string);
			safeArray.SetElement(index++, bstr.GetBuffer());
		}

		Variant variant;
		variant->vt = VT_ARRAY | VT_BSTR;
		variant->parray = safeArray.Detach();

		return variant;
	}

	IDispatch* Variant::ToIDispatch() const
	{
		if (VT_DISPATCH != m_var.vt)
		{
			THROW_VARIANT_WRONG_TYPE_EXCEPTION(VT_DISPATCH);
		}

		if (VT_NULL == m_var.vt ||
			VT_EMPTY == m_var.vt)
		{
			return nullptr;
		}

		return m_var.pdispVal;
	}

	Void Variant::MoveFrom(Variant& other)
	{
		if (&m_var != &other.m_var)
		{
			::VariantClear(&m_var);
			m_var = std::move(other.m_var);
			::VariantInit(&other.m_var);
		}
	}

	Void Variant::CopyFrom(const Variant& other)
	{
		if (&m_var != &other.m_var)
		{
			::VariantCopy(&m_var, &other.m_var);
		}
	}

} }