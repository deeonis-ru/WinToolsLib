#include "CommonMemoryStream.h"
#include "Exception.h"

namespace WinToolsLib
{
	SizeT CommonMemoryStream::GetPosition() const
	{
		return m_position;
	}

	Void CommonMemoryStream::SetPosition(SizeT position)
	{
		m_position = position;
	}

	Void CommonMemoryStream::Reset()
	{
		m_position = 0;
	}

	Void CommonMemoryStream::Skip(SizeT bytes)
	{
		m_position += bytes;
	}

	IStream& CommonMemoryStream::operator>>(Int8& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator>>(Int16& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator>>(Int32& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator>>(Int64& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& CommonMemoryStream::operator>>(UInt8& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator>>(UInt16& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator>>(UInt32& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& CommonMemoryStream::operator>>(UInt64& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& CommonMemoryStream::operator<<(Int8 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& CommonMemoryStream::operator<<(Int16 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator<<(Int32 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator<<(Int64 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& CommonMemoryStream::operator<<(UInt8 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator<<(UInt16 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator<<(UInt32 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& CommonMemoryStream::operator<<(UInt64 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
}