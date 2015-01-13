#pragma once
#include "Types.h"
#include "String.h"
#include <Rpc.h>

namespace WinToolsLib
{
	class Guid
	{
	public:
		Guid();
		Guid(const GUID& guid);
		Guid(const TChar* guid);

		Guid(Guid&& other);
		Guid(const Guid& other);

		Guid& operator=(Guid&& other);
		Guid& operator=(const Guid& other);

		Bool operator<(const Guid& other) const;
		Bool operator>(const Guid& other) const;

		Bool operator<=(const Guid& other) const;
		Bool operator>=(const Guid& other) const;

		Bool operator==(const Guid& other) const;
		Bool operator!=(const Guid& other) const;

		Bool IsNull() const;
		String ToString() const;

		operator const GUID&() const;

		static Guid Create();

	public:
		static const Guid Null;

	protected:
		Void MoveFrom(Guid& other);
		Void CopyFrom(const Guid& other);

	private:
		GUID m_guid;
	};

	inline Bool Guid::operator<(const Guid& other) const
	{
		return 0 < memcmp(&m_guid, &other.m_guid, sizeof(m_guid));
	}

	inline Bool Guid::operator>(const Guid& other) const
	{
		return !operator<=(other);
	}

	inline Bool Guid::operator<=(const Guid& other) const
	{
		return 0 <= memcmp(&m_guid, &other.m_guid, sizeof(m_guid));
	}

	inline Bool Guid::operator>=(const Guid& other) const
	{
		return !operator<(other);
	}

	inline Bool Guid::operator==(const Guid& other) const
	{
		return 0 == memcmp(&m_guid, &other.m_guid, sizeof(m_guid));
	}

	inline Bool Guid::operator!=(const Guid& other) const
	{
		return !operator==(other);
	}

	inline Bool Guid::IsNull() const
	{
		static const Guid null;
		return 0 == memcmp(&null.m_guid, &m_guid, sizeof(m_guid));
	}

	inline Guid::operator const GUID&() const
	{
		return m_guid;
	}
}
