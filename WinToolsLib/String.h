#pragma once
#include "Types.h"
#include <stdarg.h>
#include <string>
#include <locale>
#include <algorithm>
#include <functional>
#include <vector>

namespace WinToolsLib
{
	template <class Elem, class Traits>
	class StringT
	{
		typedef std::basic_string<Elem, std::char_traits<Elem>, std::allocator<Elem>> BasicString;

	public:
		StringT();
		StringT(const Elem* ptr);
		StringT(const Elem* ptr, SizeT size);
		StringT(StringT&& other);
		StringT(const StringT& other);
		
		StringT& operator=(const Elem* ptr);
		StringT& operator=(StringT&& other);
		StringT& operator=(const StringT& other);
		
		StringT& operator+=(Elem elem);
		StringT& operator+=(const Elem* ptr);
		StringT& operator+=(const StringT& other);

		StringT operator+(Elem elem) const;
		StringT operator+(const Elem* ptr) const;
		StringT operator+(const StringT& other) const;

		Bool operator==(const Elem* ptr) const;
		Bool operator==(const StringT& other) const;

		Bool operator!=(const Elem* ptr) const;
		Bool operator!=(const StringT& other) const;

		Bool operator<(const StringT& other) const;
		Bool operator>(const StringT& other) const;

		Elem& operator[](SizeT pos);
		const Elem& operator[](SizeT pos) const;

		operator const Elem*() const;

		const Elem* GetBuffer() const;
		Elem* GetBuffer();
		SizeT GetLength() const;
		SizeT GetSize() const;

		Void Resize(SizeT newSize);
		Void Reserve(SizeT size);

		SizeT Find(const Elem* ptr, SizeT from = 0) const;
		SizeT Find(const StringT& other) const;

		SizeT ReverseFind(const Elem* ptr) const;
		SizeT ReverseFind(const StringT& other) const;

		SizeT FindLast(const Elem* ptr) const;
		SizeT FindLast(const StringT& other) const;
		
		Void Replace(SizeT pos, const Elem* ptr);
		Void Replace(SizeT pos, const StringT& other);
		
		Void Replace(SizeT from, SizeT to, const Elem* ptr);
		Void Replace(SizeT from, SizeT to, const StringT& other);

		Void Replace(const Elem* what, const Elem* with);

		Void ToLower();
		Void ToUpper();

		StringT ToLower() const;
		StringT ToUpper() const;

		Bool IsEmpty() const;
		Bool IsEqual(const Elem* ptr) const;

		Bool BeginsWith(const Elem* ptr) const;
		Bool EndsWith(const Elem* ptr) const;
		Bool Contains(const Elem* ptr) const;
		Bool Match(const Elem* wildcard) const;

		StringT SubString(SizeT from) const;
		StringT SubString(SizeT from, SizeT count) const;

		SizeT Split(const StringT& by, StringT& token, SizeT& start) const;
		Void Split(const StringT& by, std::vector<StringT>& tokenList) const;

		Void Trim(const Elem* ptr);
		Void TrimLeft(const Elem* ptr);
		Void TrimRight(const Elem* ptr);

		Void Format(const Elem* format, ...);
		Void FormatV(const Elem* format, va_list argList);

		Void Append(const Elem* ptr);
		Void Append(const Elem* ptr, SizeT count);
		Void Append(SizeT count, Elem elem);
		Void Append(const StringT& other);

		static SizeT GetLength(const Elem* ptr);

		static StringT FormatS(const Elem* format, ...);

		static StringT Convert(Int32 value);
		static StringT Convert(Int64 value);
		static StringT Convert(UInt32 value);
		static StringT Convert(UInt64 value);
		static StringT Convert(Double value);

		template <class OtherElem, class OtherTraits>
		static StringT Convert(const StringT<OtherElem, OtherTraits>& other);

	protected:
		Void MoveFrom(StringT& other);
		Void CopyFrom(const StringT& other);

	private:
		BasicString m_basic;
	};

	template <class Elem, class Traits>
	StringT<Elem, Traits>::StringT()
	{
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>::StringT(const Elem* ptr) :
		m_basic(ptr)
	{
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>::StringT(const Elem* ptr, SizeT size) :
		m_basic(ptr, size)
	{
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>::StringT(StringT<Elem, Traits>&& other)
	{
		MoveFrom(other);
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>::StringT(const StringT<Elem, Traits>& other)
	{
		CopyFrom(other);
	}
		
	template <class Elem, class Traits>
	StringT<Elem, Traits>& StringT<Elem, Traits>::operator=(const Elem* ptr)
	{
		m_basic = ptr;
		return *this;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>& StringT<Elem, Traits>::operator=(StringT<Elem, Traits>&& other)
	{
		MoveFrom(other);
		return *this;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>& StringT<Elem, Traits>::operator=(const StringT<Elem, Traits>& other)
	{
		CopyFrom(other);
		return *this;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>& StringT<Elem, Traits>::operator+=(Elem elem)
	{
		m_basic += elem;
		return *this;
	}
		
	template <class Elem, class Traits>
	StringT<Elem, Traits>& StringT<Elem, Traits>::operator+=(const Elem* ptr)
	{
		m_basic += ptr;
		return *this;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>& StringT<Elem, Traits>::operator+=(const StringT<Elem, Traits>& other)
	{
		m_basic += other.m_basic;
		return *this;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::operator+(Elem elem) const
	{
		return StringT(m_basic + elem);
	}
		
	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::operator+(const Elem* ptr) const
	{
		BasicString tmp = m_basic + ptr;
		return StringT(tmp.c_str());
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::operator+(const StringT<Elem, Traits>& other) const
	{
		BasicString tmp = m_basic + other.m_basic;
		return StringT(tmp.c_str());
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::operator==(const Elem* ptr) const
	{
		if (nullptr == ptr)
		{
			return m_basic.empty();
		}
		return m_basic == ptr;
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::operator==(const StringT<Elem, Traits>& other) const
	{
		return m_basic == other.m_basic;
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::operator!=(const Elem* ptr) const
	{
		return !operator==(ptr);
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::operator!=(const StringT<Elem, Traits>& other) const
	{
		return !operator==(other);
	}

	template <class Elem, class Traits>
	Bool operator==(const Elem* left, const StringT<Elem, Traits>& right)
	{
		return right.IsEqual(left);
	}

	template <class Elem, class Traits>
	Bool operator!=(const Elem* left, const StringT<Elem, Traits>& right)
	{
		return !right.IsEqual(left);
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::operator<(const StringT<Elem, Traits>& other) const
	{
		return m_basic < other.m_basic;
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::operator>(const StringT<Elem, Traits>& other) const
	{
		return m_basic > other.m_basic;
	}

	template <class Elem, class Traits>
	Elem& StringT<Elem, Traits>::operator[](SizeT pos)
	{
		return m_basic[pos];
	}

	template <class Elem, class Traits>
	const Elem& StringT<Elem, Traits>::operator[](SizeT pos) const
	{
		return m_basic[pos];
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits>::operator const Elem*() const
	{
		return m_basic.c_str();
	}

	template <class Elem, class Traits>
	const Elem* StringT<Elem, Traits>::GetBuffer() const
	{
		if (m_basic.empty())
		{
			return nullptr;
		}
		return m_basic.c_str();
	}

	template <class Elem, class Traits>
	Elem* StringT<Elem, Traits>::GetBuffer()
	{
		if (m_basic.empty())
		{
			return nullptr;
		}
		return &m_basic[0];
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::GetLength() const
	{
		return m_basic.size();
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::GetSize() const
	{
		return m_basic.size();
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Resize(SizeT newSize)
	{
		m_basic.resize(newSize);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Reserve(SizeT size)
	{
		m_basic.reserve(size);
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::Find(const Elem* ptr, SizeT from) const
	{
		return m_basic.find(ptr, from);
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::Find(const StringT<Elem, Traits>& other) const
	{
		return m_basic.find(other);
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::ReverseFind(const Elem* ptr) const
	{
		return m_basic.rfind(ptr);
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::ReverseFind(const StringT<Elem, Traits>& other) const
	{
		return m_basic.rfind(other);
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::FindLast(const Elem* ptr) const
	{
		return m_basic.find_last_of(ptr);
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::FindLast(const StringT<Elem, Traits>& other) const
	{
		return m_basic.find_last_of(other);
	}
		
	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Replace(SizeT pos, const Elem* ptr)
	{
		auto size = m_basic.size();
		if (pos < size)
		{
			m_basic.replace(pos, size - pos, ptr);
		}
	}
		
	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Replace(SizeT pos, const StringT<Elem, Traits>& other)
	{
		auto size = m_basic.size();
		if (pos < size)
		{
			m_basic.replace(pos, size - pos, other.m_basic);
		}
	}
		
	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Replace(SizeT from, SizeT to, const Elem* ptr)
	{
		auto first = m_basic.begin() + from;
		auto last = m_basic.begin() + to;
		m_basic.replace(first, last, ptr);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Replace(SizeT from, SizeT to, const StringT<Elem, Traits>& other)
	{
		auto first = m_basic.begin() + from;
		auto last = m_basic.begin() + to;
		m_basic.replace(first, last, other);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Replace(const Elem* what, const Elem* with)
	{
		if (!what || !with)
		{
			return;
		}

		auto pos = m_basic.find(what);
		if (-1 == pos)
		{
			return;
		}

		const auto whatLength = Traits::GetLength(what);
		const auto withLength = Traits::GetLength(with);

		while (-1 != pos)
		{
			m_basic.replace(pos, whatLength, with);
			pos = m_basic.find(what, pos + withLength);
		}
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::ToLower()
	{
		std::transform(
			begin(m_basic),
			end(m_basic),
			begin(m_basic),
			std::bind2nd(std::ptr_fun(&std::tolower<Elem>), std::locale("")));
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::ToUpper()
	{
		std::transform(
			begin(m_basic),
			end(m_basic),
			begin(m_basic),
			std::bind2nd(std::ptr_fun(&std::toupper<Elem>), std::locale("")));
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::ToLower() const
	{
		StringT<Elem, Traits> copy(*this);
		copy.ToLower();
		return copy;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::ToUpper() const
	{
		StringT<Elem, Traits> copy(*this);
		copy.ToUpper();
		return copy;
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::IsEmpty() const
	{
		return m_basic.empty();
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::IsEqual(const Elem* ptr) const
	{
		if (nullptr == ptr)
		{
			return m_basic.empty();
		}
		return 0 == m_basic.compare(ptr);
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::BeginsWith(const Elem* ptr) const
	{
		return 0 == m_basic.find(ptr);
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::EndsWith(const Elem* what) const
	{
		const auto whatLength = Traits::GetLength(what);
		if (0 == whatLength)
		{
			return True;
		}
		if (m_basic.size() < whatLength)
		{
			return False;
		}
		const auto pos = m_basic.find(what);
		return pos == (m_basic.size() - whatLength);
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::Contains(const Elem* ptr) const
	{
		return -1 != m_basic.find(ptr);
	}

	template <class Elem, class Traits>
	Bool StringT<Elem, Traits>::Match(const Elem* wild) const
	{
		const Elem* string = m_basic.c_str();

		const Elem* cp = nullptr;
		const Elem* mp = nullptr;

		while ((*string) && (*wild != Traits::star))
		{
			if ((*wild != *string) &&
				(std::tolower(*wild, std::locale::classic()) !=
				std::tolower(*string, std::locale::classic())) &&
				(*wild != Traits::question))
			{
				return False;
			}
			wild++;
			string++;
		}

		while (*string)
		{
			if (*wild == Traits::star)
			{
				if (!*++wild)
				{
					return True;
				}
				mp = wild;
				cp = string+1;
			}
			else if (
				(*wild == *string) ||
				(std::tolower(*wild, std::locale::classic()) ==
				std::tolower(*string, std::locale::classic())) ||
				(*wild == Traits::question))
			{
				wild++;
				string++;
			}
			else
			{
				wild = mp;
				string = cp++;
			}
		}

		while (*wild == Traits::star)
		{
			wild++;
		}

		if (*wild)
		{
			return False;
		}

		return True;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::SubString(SizeT from) const
	{
		StringT subString;
		subString.m_basic = m_basic.substr(from);
		return subString;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::SubString(SizeT from, SizeT count) const
	{
		StringT subString;
		subString.m_basic = m_basic.substr(from, count);
		return subString;
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::Split(const StringT& by, StringT& token, SizeT& start) const
	{
		size_t pos = -1;
		auto stringTailToToken = [&]()
		{
			token = SubString(start);
			start += token.GetLength();
			pos = m_basic.size();
		};

		if (by.IsEmpty())
		{
			if (start < m_basic.size())
				stringTailToToken();
			return pos;
		}

		pos = Find(by, start);
		if (-1 != pos)
		{
			token = SubString(start, pos - start);
			start += token.GetLength() + by.GetLength();
		}
		else if (start < m_basic.size())
		{
			stringTailToToken();
		}
		return pos;
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Split(const StringT& by, std::vector<StringT>& tokenList) const
	{
		StringT token;
		SizeT start = 0;

		auto pos = Split(by, token, start);
		while (-1 != pos)
		{
			if (!token.IsEmpty())
				tokenList.push_back(token);
			pos = Split(by, token, start);
		}
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Trim(const Elem* ptr)
	{
		TrimLeft(ptr);
		TrimRight(ptr);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::TrimLeft(const Elem* ptr)
	{
		if (nullptr == ptr)
		{
			return;
		}
		const auto pos = m_basic.find_first_not_of(ptr);
		if (pos != BasicString::npos)
		{
			m_basic.erase(0, pos);
		}
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::TrimRight(const Elem* ptr)
	{
		if (nullptr == ptr)
		{
			return;
		}
		const auto pos = m_basic.find_last_not_of(ptr);
		if (pos != BasicString::npos)
		{
			m_basic.erase(pos + 1);
		}
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Format(const Elem* format, ...)
	{
		va_list argList;
		va_start(argList, format);

		FormatV(format, argList);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::FormatV(const Elem* format, va_list argList)
	{
		auto size = Traits::CountPrintF(format, argList);
		m_basic.resize(size);
		Traits::PrintF(&m_basic[0], size + 1, format, argList);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Append(const Elem* ptr)
	{
		m_basic.append(ptr);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Append(const Elem* ptr, SizeT count)
	{
		m_basic.append(ptr, count);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Append(SizeT count, Elem elem)
	{
		m_basic.append(count, elem);
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::Append(const StringT& other)
	{
		m_basic.append(other.m_basic);
	}

	template <class Elem, class Traits>
	SizeT StringT<Elem, Traits>::GetLength(const Elem* ptr)
	{
		return Traits::GetLength(ptr);
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::FormatS(const Elem* format, ...)
	{
		va_list argList;
		va_start(argList, format);

		StringT result;
		result.FormatV(format, argList);
		return result;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::Convert(Int32 value)
	{
		StringT result;
		result.Format(Traits::formatInt32, value);
		return result;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::Convert(Int64 value)
	{
		StringT result;
		result.Format(Traits::formatInt64, value);
		return result;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::Convert(UInt32 value)
	{
		StringT result;
		result.Format(Traits::formatUInt32, value);
		return result;
	}

	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::Convert(UInt64 value)
	{
		StringT result;
		result.Format(Traits::formatUInt64, value);
		return result;
	}
		
	template <class Elem, class Traits>
	StringT<Elem, Traits> StringT<Elem, Traits>::Convert(Double value)
	{
		StringT result;
		result.Format(Traits::formatDouble, value);
		return result;
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::MoveFrom(StringT<Elem, Traits>& other)
	{
		if (this != &other)
		{
			m_basic = std::move(other.m_basic);
		}
	}

	template <class Elem, class Traits>
	Void StringT<Elem, Traits>::CopyFrom(const StringT<Elem, Traits>& other)
	{
		if (this != &other)
		{
			m_basic = other.m_basic;
		}
	}

	template <class Elem>
	struct CharTraits
	{
		typedef Elem CharType;

		static const Elem star;
		static const Elem question;

		static const Elem* const formatInt32;
		static const Elem* const formatInt64;
		static const Elem* const formatUInt32;
		static const Elem* const formatUInt64;
		static const Elem* const formatDouble;

		static SizeT GetLength(const Elem* ptr);
		static SizeT CountPrintF(const Elem* format, va_list argList);
		static SizeT PrintF(Elem* buffer, SizeT size, const Elem* format, va_list argList);
	};

	template<>
	struct CharTraits<Char>
	{
		typedef Char Elem;
		typedef Elem CharType;

		static const Elem star;
		static const Elem question;

		static const Elem* const formatInt32;
		static const Elem* const formatInt64;
		static const Elem* const formatUInt32;
		static const Elem* const formatUInt64;
		static const Elem* const formatDouble;

		static SizeT GetLength(const Elem* ptr);
		static SizeT CountPrintF(const Elem* format, va_list argList);
		static SizeT PrintF(Elem* buffer, SizeT size, const Elem* format, va_list argList);
	};

	template<>
	struct CharTraits<Byte>
	{
		typedef Byte Elem;
		typedef Elem CharType;

		static const Elem star;
		static const Elem question;

		static const Elem* const formatInt32;
		static const Elem* const formatInt64;
		static const Elem* const formatUInt32;
		static const Elem* const formatUInt64;
		static const Elem* const formatDouble;

		static SizeT GetLength(const Elem* ptr);
		static SizeT CountPrintF(const Elem* format, va_list argList);
		static SizeT PrintF(Elem* buffer, SizeT size, const Elem* format, va_list argList);
	};

	template<>
	struct CharTraits<WChar>
	{
		typedef WChar Elem;
		typedef Elem CharType;

		static const Elem star;
		static const Elem question;

		static const Elem* const formatInt32;
		static const Elem* const formatInt64;
		static const Elem* const formatUInt32;
		static const Elem* const formatUInt64;
		static const Elem* const formatDouble;

		static SizeT GetLength(const Elem* ptr);
		static SizeT CountPrintF(const Elem* format, va_list argList);
		static SizeT PrintF(Elem* buffer, SizeT size, const Elem* format, va_list argList);
	};

	typedef StringT<Char, CharTraits<Char>>   StringA;
	typedef StringT<Byte, CharTraits<Byte>>   StringB;
	typedef StringT<WChar, CharTraits<WChar>> StringW;
	typedef StringT<TChar, CharTraits<TChar>> String;
}
