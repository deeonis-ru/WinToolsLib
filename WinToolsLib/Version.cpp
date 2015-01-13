#include "Version.h"
#include "Exception.h"

#include <ctype.h>
#include <stdlib.h>
#include <tchar.h>

namespace WinToolsLib
{
	Version::Version()
	{
	}

	Version::Version(Version&& other)
	{
		MoveFrom(other);
	}

	Version::Version(const Version& other)
	{
		CopyFrom(other);
	}

	Version::Version(const String& string)
	{
		String delim(Text("."));
		String part;
		SizeT start = 0;

		auto pos = string.Split(delim, part, start);
		while (-1 != pos)
		{
			SizeT value = 0;

			auto raw = part.GetBuffer();
			if (nullptr != raw)
			{
				while (*raw != Text('\0') && !_istdigit(*raw))
				{
					raw++;
				}
				value = _tcstoul(raw, NULL, 10);
			}

			m_parts.push_back(value);
			pos = string.Split(delim, part, start);
		}
	}

	Version& Version::operator=(Version&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Version& Version::operator=(const Version& other)
	{
		CopyFrom(other);
		return *this;
	}

	Bool Version::operator<(const Version& other) const
	{
		const auto shortes = __min(m_parts.size(), other.GetPartsCount());
		for (SizeT i = 0; i < shortes; i++)
		{
			if (m_parts[i] < other[i])
			{
				return True;
			}
			if (m_parts[i] > other[i])
			{
				return False;
			}
		}

		if (m_parts.size() > other.GetPartsCount())
		{
			return False;
		}

		for (SizeT i = shortes; i < other.GetPartsCount(); i++)
		{
			if (other[i] != 0)
			{
				return True;
			}
		}

		return False;
	}

	Bool Version::operator>(const Version& other) const
	{
		const auto shortes = __min(m_parts.size(), other.GetPartsCount());
		for (SizeT i = 0; i < shortes; i++)
		{
			if (m_parts[i] > other[i])
			{
				return True;
			}
			if (m_parts[i] < other[i])
			{
				return False;
			}
		}

		if (other.GetPartsCount() > m_parts.size())
		{
			return False;
		}

		for (SizeT i = shortes; i < m_parts.size(); i++)
		{
			if (m_parts[i] != 0)
			{
				return True;
			}
		}

		return False;
	}

	Bool Version::operator<=(const Version& other) const
	{
		return !operator>(other);
	}

	Bool Version::operator>=(const Version& other) const
	{
		return !operator<(other);
	}

	Bool Version::operator==(const Version& other) const
	{
		const auto shortes = __min(m_parts.size(), other.GetPartsCount());
		for (SizeT i = 0; i < shortes; i++)
		{
			if (m_parts[i] != other[i])
			{
				return False;
			}
		}

		for (SizeT i = shortes; i < m_parts.size(); i++)
		{
			if (m_parts[i] != 0)
			{
				return False;
			}
		}

		for (SizeT i = shortes; i < other.GetPartsCount(); i++)
		{
			if (other[i] != 0)
			{
				return False;
			}
		}

		return True;
	}

	Bool Version::operator!=(const Version& other) const
	{
		return !operator==(other);
	}

	String Version::ToString() const
	{
		String version;
		for (const auto& part : m_parts)
		{
			if (!version.IsEmpty())
			{
				version += Text(".");
			}
			version += String::Convert(part);
		}
		return version;
	}

	Int32 Version::ToInt() const
	{
		Int32 version = 0;
		if (m_parts.size() >= 3)
		{
			auto distance = (Int32)m_parts[2];
			auto minor = (Int32)m_parts[1];
			auto major = (Int32)m_parts[0];
		
			version = ((distance & 0XFF) << 8) |
				((minor & 0xFF) << 16) |
				((major & 0xFF) << 24);
		}
		return version;
	}

	Void Version::MoveFrom(Version& other)
	{
		if (this != &other)
		{
			m_parts = std::move(other.m_parts);
		}
	}

	Void Version::CopyFrom(const Version& other)
	{
		if (this != &other)
		{
			m_parts = other.m_parts;
		}
	}
}