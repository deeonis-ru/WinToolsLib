#include "Stream.h"
#include "Exception.h"

namespace WinToolsLib
{
	Stream::Stream(Buffer& buffer) :
		m_buffer(buffer),
		m_position(0)
	{
	}

	Stream::Stream(Buffer& buffer, SizeT offset) :
		m_buffer(buffer),
		m_position(offset)
	{
	}

	SizeT Stream::GetPosition() const
	{
		return m_position;
	}

	Void Stream::SetPosition(SizeT position)
	{
		m_position = position;
	}

	Void Stream::Reset()
	{
		m_position = 0;
	}

	Void Stream::Skip(SizeT bytes)
	{
		if (m_position + bytes > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}
		m_position += bytes;
	}

	Void Stream::Read(PByte buffer, SizeT bytes)
	{
		if (m_position + bytes > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		for (SizeT i = 0; i < bytes; i++)
		{
			buffer[i] = m_buffer.GetBuffer()[m_position++];
		}
	}

	Void Stream::Write(PByte buffer, SizeT bytes)
	{
		if (m_position + bytes > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		for (SizeT i = 0; i < bytes; i++)
		{
			m_buffer[m_position++] = buffer[i];
		}
	}

	IStream& Stream::operator>>(Int8& value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		value = *(Int8*)(m_buffer.GetBuffer() + m_position);
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator>>(Int16& value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		value = *(Int16*)(m_buffer.GetBuffer() + m_position);
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator>>(Int32& value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		value = *(Int32*)(m_buffer.GetBuffer() + m_position);
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator>>(Int64& value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		value = *(Int64*)(m_buffer.GetBuffer() + m_position);
		m_position += sizeof(value);

		return *this;
	}

	IStream& Stream::operator>>(UInt8& value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		value = *(UInt8*)(m_buffer.GetBuffer() + m_position);
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator>>(UInt16& value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		value = *(UInt16*)(m_buffer.GetBuffer() + m_position);
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator>>(UInt32& value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		value = *(UInt32*)(m_buffer.GetBuffer() + m_position);
		m_position += sizeof(value);

		return *this;
	}

	IStream& Stream::operator>>(UInt64& value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		value = *(UInt64*)(m_buffer.GetBuffer() + m_position);
		m_position += sizeof(value);

		return *this;
	}

	IStream& Stream::operator<<(Int8 value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		*(Int8*)(m_buffer.GetBuffer() + m_position) = value;
		m_position += sizeof(value);

		return *this;
	}

	IStream& Stream::operator<<(Int16 value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		*(Int16*)(m_buffer.GetBuffer() + m_position) = value;
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator<<(Int32 value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		*(Int32*)(m_buffer.GetBuffer() + m_position) = value;
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator<<(Int64 value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		*(Int64*)(m_buffer.GetBuffer() + m_position) = value;
		m_position += sizeof(value);

		return *this;
	}

	IStream& Stream::operator<<(UInt8 value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		*(UInt8*)(m_buffer.GetBuffer() + m_position) = value;
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator<<(UInt16 value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		*(UInt16*)(m_buffer.GetBuffer() + m_position) = value;
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator<<(UInt32 value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		*(UInt32*)(m_buffer.GetBuffer() + m_position) = value;
		m_position += sizeof(value);

		return *this;
	}
	
	IStream& Stream::operator<<(UInt64 value)
	{
		if (m_position + sizeof(value) > m_buffer.GetSize())
		{
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		*(UInt64*)(m_buffer.GetBuffer() + m_position) = value;
		m_position += sizeof(value);

		return *this;
	}
}