#pragma once
#include "Types.h"
#include "String.h"

#include <vector>

namespace WinToolsLib
{
	class Version
	{
	public:
		Version();
		Version(Version&& other);
		Version(const Version& other);
		Version(const String& string);

		Version& operator=(Version&& other);
		Version& operator=(const Version& other);

		Bool operator<(const Version& other) const;
		Bool operator>(const Version& other) const;

		Bool operator<=(const Version& other) const;
		Bool operator>=(const Version& other) const;

		Bool operator==(const Version& other) const;
		Bool operator!=(const Version& other) const;

		const SizeT& operator[](SizeT index) const;
		SizeT& operator[](SizeT index);

		SizeT GetPartsCount() const;

		String ToString() const;
		Int32 ToInt() const;

	protected:
		Void MoveFrom(Version& other);
		Void CopyFrom(const Version& other);

	private:
		std::vector<SizeT> m_parts;
	};

	inline const SizeT& Version::operator[](SizeT index) const
	{
		return m_parts[index];
	}

	inline SizeT& Version::operator[](SizeT index)
	{
		return m_parts[index];
	}

	inline SizeT Version::GetPartsCount() const
	{
		return m_parts.size();
	}
}
