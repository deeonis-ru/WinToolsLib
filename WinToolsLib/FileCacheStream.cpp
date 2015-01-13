#include "FileCacheStream.h"
#include "Exception.h"

namespace WinToolsLib
{
	FileCacheStream::FileCacheStream(const String& path, UInt32 chunkSize) :
		m_chunkSize(chunkSize),
		m_position(0),
		m_chunkBegin(0),
		m_chunkEnd(0),
		m_currentChunk(0)
	{
		m_file = File::Open(path);
		m_fileSize = m_file.GetSize();
		
		auto n = m_fileSize / chunkSize;
		if (n * chunkSize < m_fileSize)
		{
			n++;
		}
		m_chunks.resize((SizeT)n);
	}

	SizeT FileCacheStream::GetPosition() const
	{
		return m_position;
	}

	Void FileCacheStream::SetPosition(SizeT position)
	{
		m_position = position;
	}

	Void FileCacheStream::Reset()
	{
		m_position = 0;
	}

	Void FileCacheStream::Skip(SizeT bytes)
	{
		m_position += bytes;
	}

	Void FileCacheStream::Read(PByte buffer, SizeT bytes)
	{
		SizeT i = 0;
		while (bytes)
		{
			auto accessible = GetAccessible();
			auto min = min(accessible, bytes);
			auto offset = m_position - m_chunkBegin;

			CopyMemory(buffer + i, m_chunks[m_currentChunk]->GetBuffer() + offset, min);
			
			i += min;
			m_position += min;
			bytes -= min;
		}
	}

	Void FileCacheStream::Write(PByte buffer, SizeT bytes)
	{
		SizeT i = 0;
		while (bytes)
		{
			auto accessible = GetAccessible();
			auto min = min(accessible, bytes);
			auto offset = m_position - m_chunkBegin;

			CopyMemory(m_chunks[m_currentChunk]->GetBuffer() + offset, buffer + i, min);
			
			i += min;
			m_position += min;
			bytes -= min;
		}
	}

	IStream& FileCacheStream::operator>>(Int8& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator>>(Int16& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator>>(Int32& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator>>(Int64& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& FileCacheStream::operator>>(UInt8& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator>>(UInt16& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator>>(UInt32& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& FileCacheStream::operator>>(UInt64& value)
	{
		Read((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& FileCacheStream::operator<<(Int8 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& FileCacheStream::operator<<(Int16 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator<<(Int32 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator<<(Int64 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}

	IStream& FileCacheStream::operator<<(UInt8 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator<<(UInt16 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator<<(UInt32 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}
	
	IStream& FileCacheStream::operator<<(UInt64 value)
	{
		Write((PByte)&value, sizeof(value));
		return *this;
	}

	SizeT FileCacheStream::GetAccessible()
	{
		if (m_position >= m_chunkBegin && m_position < m_chunkEnd)
		{
			// Position is inside of the current chunk
			return m_chunkEnd - m_position;
		}

		if (m_position >= m_fileSize)
		{
			// Position is too big
			THROW_OUT_OF_RANGE_EXCEPTION();
		}

		m_currentChunk = m_position / m_chunkSize;
		m_chunkBegin = m_currentChunk * m_chunkSize;

		if (nullptr == m_chunks[m_currentChunk])
		{
			// Chunk is not loaded yet
			m_file.SetFilePointer(m_chunkBegin);
			auto buffer = m_file.Read(m_chunkSize);

			m_chunks[m_currentChunk].reset(new Buffer(std::move(buffer)));
		}

		m_chunkEnd = m_chunkBegin + m_chunks[m_currentChunk]->GetSize();
		return m_chunkEnd - m_position;
	}
}