#include "Buffer.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

namespace WinToolsLib
{
	Buffer::Buffer(SizeT size) :
		m_buffer(nullptr),
		m_size(size)
	{
		if (m_size > 0)
		{
			m_buffer = Allocate(m_size);
			ZeroMemory(m_buffer, m_size);
		}
	}

	Buffer::Buffer(PCByte buffer, SizeT size) :
		m_buffer(nullptr),
		m_size(size)
	{
		if (m_size > 0)
		{
			m_buffer = Allocate(m_size);
			CopyMemory(m_buffer, buffer, m_size);
		}
	}

	Buffer::Buffer(const Buffer& other) :
		m_buffer(nullptr),
		m_size(other.m_size)
	{
		if (m_size > 0)
		{
			m_buffer = Allocate(m_size);
			CopyMemory(m_buffer, other.m_buffer, m_size);
		}
	}

	Buffer::Buffer(Buffer&& other) :
		m_buffer(other.m_buffer),
		m_size(other.m_size)
	{
		other.m_buffer = nullptr;
		other.m_size = 0;
	}

	Buffer::~Buffer()
	{
		Free();
	}

	Buffer& Buffer::operator=(Buffer&& other)
	{
		if (this != &other)
		{
			Free();

			m_buffer = other.m_buffer;
			m_size = other.m_size;

			other.m_buffer = nullptr;
			other.m_size = 0;
		}

		return *this;
	}

	Buffer Buffer::operator+(const Buffer& other) const
	{
		Buffer sum(m_size + other.GetSize());
		CopyMemory(sum.GetBuffer(), m_buffer, m_size);
		CopyMemory(sum.GetBuffer() + m_size, other.GetBuffer(), other.GetSize());
		return sum;
	}

	Buffer& Buffer::operator+=(const Buffer& other)
	{
		Buffer sum(m_size + other.GetSize());
		CopyMemory(sum.GetBuffer(), m_buffer, m_size);
		CopyMemory(sum.GetBuffer() + m_size, other.GetBuffer(), other.GetSize());
		*this = std::move(sum);
		return *this;
	}

	Buffer& Buffer::Append(PCByte buffer, SizeT size)
	{
		Buffer sum(m_size + size);
		CopyMemory(sum.GetBuffer(), m_buffer, m_size);
		CopyMemory(sum.GetBuffer() + m_size, buffer, size);
		*this = std::move(sum);
		return *this;
	}

	String Buffer::ToString(const Buffer& source)
	{
		auto buffer = source.GetBuffer();
		auto size = source.GetSize();

		String result;
		result.Reserve(size * 2);

		TChar byte[3];
		for (SizeT i = 0; i < size; i++)
		{
			_stprintf_s(byte, Text("%02X"), (UInt32)buffer[i]);
			result.Append(byte);
		}
		return result;
	}

	Buffer Buffer::FromString(const String& source)
	{
		auto buffer = source.GetBuffer();
		auto size = source.GetLength() / 2;

		Buffer result(size);
		for (SizeT i = 0; i < size; i++)
		{
			UInt32 byte = 0;
			_stscanf_s(buffer + i * 2, Text("%02X"), &byte);
			result.GetBuffer()[i] = byte;
		}

		return result;
	}

	PByte Buffer::Allocate(SizeT size)
	{
		return new Byte[size];
	}

	Void Buffer::Free()
	{
		if (m_buffer)
		{
			delete[] m_buffer;
			m_buffer = nullptr;
		}
		m_size = 0;
	}
}
