#include "Guid.h"
#include <ObjBase.h>

#include "Exception.h"
#pragma comment (lib, "Rpcrt4.lib")

namespace WinToolsLib
{
	const Guid Guid::Null;

	Guid::Guid()
	{
		ZeroMemory(&m_guid, sizeof(m_guid));
	}

	Guid::Guid(const GUID& guid)
	{
		CopyMemory(&m_guid, &guid, sizeof(m_guid));
	}

	Guid::Guid(const TChar* guid)
	{
		ZeroMemory(&m_guid, sizeof(m_guid));

		auto error = ::UuidFromString((RPC_WSTR)guid, &m_guid);
		if (error != RPC_S_OK)
		{
			THROW_WIN32_EXCEPTION(error);
		}
	}

	Guid::Guid(Guid&& other)
	{
		MoveFrom(other);
	}

	Guid::Guid(const Guid& other)
	{
		CopyFrom(other);
	}

	Guid& Guid::operator=(Guid&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Guid& Guid::operator=(const Guid& other)
	{
		CopyFrom(other);
		return *this;
	}

	String Guid::ToString() const
	{
		String string;
		string.Format(
			Text("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
			m_guid.Data1,
			m_guid.Data2,
			m_guid.Data3,
			m_guid.Data4[0],
			m_guid.Data4[1],
			m_guid.Data4[2],
			m_guid.Data4[3],
			m_guid.Data4[4],
			m_guid.Data4[5],
			m_guid.Data4[6],
			m_guid.Data4[7]);
		return string;
	}

	Guid Guid::Create()
	{
		Guid guid;
		::CoCreateGuid(&guid.m_guid);
		return guid;
	}

	Void Guid::MoveFrom(Guid& other)
	{
		if (this != &other)
		{
			CopyMemory(&m_guid, &other.m_guid, sizeof(m_guid));
			ZeroMemory(&other.m_guid, sizeof(m_guid));
		}
	}

	Void Guid::CopyFrom(const Guid& other)
	{
		if (this != &other)
		{
			CopyMemory(&m_guid, &other.m_guid, sizeof(m_guid));
		}
	}
}