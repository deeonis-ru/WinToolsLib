#pragma once
#include "..\Types.h"
#include "..\String.h"

#include <Windows.h>

namespace WinToolsLib { namespace Com
{
	class BStr
	{
	public:
		BStr(const OLECHAR* bstr = nullptr);
		BStr(UInt32 length);
		BStr(BStr&& other);
		BStr(const BStr& other);
		~BStr();

		static BStr Alloc(UInt32 length);
		Void Free();

		Void Attach(BSTR bstr);
		BSTR Detach();

		UInt32 GetLength() const;
		BSTR GetBuffer() const;

		operator BSTR() const;
		BSTR* operator&();

		BStr& operator=(BStr&& other);
		BStr& operator=(const BStr& other);

	protected:
		Void MoveFrom(BStr& other);
		Void CopyFrom(const BStr& other);

	private:
		BSTR m_bstr;
	};

	inline BSTR BStr::GetBuffer() const
	{
		return m_bstr;
	}

	inline BStr::operator BSTR() const
	{
		return m_bstr;
	}

	inline BSTR* BStr::operator&()
	{
		return &m_bstr;
	}

} }

