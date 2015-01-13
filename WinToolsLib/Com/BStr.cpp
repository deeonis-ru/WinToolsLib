#include "BStr.h"

#include <OleAuto.h>
#pragma comment(lib, "OleAut32.lib")

#include <type_traits>

namespace WinToolsLib { namespace Com
{
	BStr::BStr(const OLECHAR* bstr) :
		m_bstr(nullptr)
	{
		m_bstr = ::SysAllocString(bstr);
	}

	BStr::BStr(UInt32 length) :
		m_bstr(nullptr)
	{
		m_bstr = ::SysAllocStringLen(nullptr, length);
	}

	BStr::BStr(BStr&& other) :
		m_bstr(nullptr)
	{
		MoveFrom(other);
	}

	BStr::BStr(const BStr& other) :
		m_bstr(nullptr)
	{
		CopyFrom(other);
	}

	BStr::~BStr()
	{
		Free();
	}

	BStr BStr::Alloc(UInt32 length)
	{
		return BStr(length);
	}

	Void BStr::Free()
	{
		::SysFreeString(Detach());
	}

	Void BStr::Attach(BSTR bstr)
	{
		Free();
		m_bstr = bstr;
	}

	BSTR BStr::Detach()
	{
		auto bstr = m_bstr;
		m_bstr = nullptr;
		return bstr;
	}

	UInt32 BStr::GetLength() const
	{
		return ::SysStringLen(m_bstr);
	}

	BStr& BStr::operator=(BStr&& other)
	{
		MoveFrom(other);
		return *this;
	}

	BStr& BStr::operator=(const BStr& other)
	{
		CopyFrom(other);
		return *this;
	}

	Void BStr::MoveFrom(BStr& other)
	{
		if (&m_bstr != &other.m_bstr)
		{
			Attach(other.Detach());
		}
	}

	Void BStr::CopyFrom(const BStr& other)
	{
		if (&m_bstr != &other.m_bstr)
		{
			::SysReAllocString(&m_bstr, other);
		}
	}

} }