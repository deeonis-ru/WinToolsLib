#pragma once
#include "Types.h"
#include "String.h"
#include "Exception.h"

namespace WinToolsLib
{
	class Buffer
	{
	public:
		Buffer(SizeT size = 0);
		Buffer(PCByte buffer, SizeT size);
		Buffer(const Buffer& other);
		Buffer(Buffer&& other);
		virtual ~Buffer();

		operator PCByte() const;
		operator PByte();

		const Byte& operator[](SizeT index) const;
		Byte& operator[](SizeT index);

		PCByte GetBuffer() const;
		PByte GetBuffer();

		SizeT GetSize() const;
		Bool IsEmpty() const;

		Buffer& operator=(Buffer&& other);
		Buffer operator+(const Buffer& other) const;
		Buffer& operator+=(const Buffer& other);

		Buffer& Append(PCByte buffer, SizeT size);
		
		static String ToString(const Buffer& source);
		static Buffer FromString(const String& source);
		
	protected:
		PByte Allocate(SizeT size);
		Void Free();

	private:
		PByte m_buffer;
		SizeT m_size;
	};

	inline Buffer::operator PCByte() const
	{
		return m_buffer;
	}

	inline Buffer::operator PByte()
	{
		return m_buffer;
	}

	inline const Byte& Buffer::operator[](SizeT index) const
	{
		if (index < m_size)
		{
			return m_buffer[index];
		}
		THROW_OUT_OF_RANGE_EXCEPTION();
	}

	inline Byte& Buffer::operator[](SizeT index)
	{
		if (index < m_size)
		{
			return m_buffer[index];
		}
		THROW_OUT_OF_RANGE_EXCEPTION();
	}

	inline PCByte Buffer::GetBuffer() const
	{
		return m_buffer;
	}

	inline PByte Buffer::GetBuffer()
	{
		return m_buffer;
	}

	inline SizeT Buffer::GetSize() const
	{
		return m_size;
	}

	inline Bool Buffer::IsEmpty() const
	{
		return 0 == m_size;
	}
}
