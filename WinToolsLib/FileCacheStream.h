#pragma once
#include "Types.h"
#include "FileSystem\File.h"
#include "String.h"
#include "IStream.h"
#include "Buffer.h"
#include <memory>
#include <vector>

namespace WinToolsLib
{
	using namespace FileSystem;

	class FileCacheStream : public IStream
	{
	public:
		FileCacheStream(const String& path, UInt32 chunkSize);
		
		SizeT GetPosition() const override;
		Void SetPosition(SizeT position) override;
		
		Void Reset() override;
		Void Skip(SizeT bytes) override;

		Void Read(PByte buffer, SizeT bytes) override;
		Void Write(PByte buffer, SizeT bytes) override;

		IStream& operator>>(Int8& value) override;
		IStream& operator>>(Int16& value) override;
		IStream& operator>>(Int32& value) override;
		IStream& operator>>(Int64& value) override;

		IStream& operator>>(UInt8& value) override;
		IStream& operator>>(UInt16& value) override;
		IStream& operator>>(UInt32& value) override;
		IStream& operator>>(UInt64& value) override;
		
		IStream& operator<<(Int8 value) override;
		IStream& operator<<(Int16 value) override;
		IStream& operator<<(Int32 value) override;
		IStream& operator<<(Int64 value) override;

		IStream& operator<<(UInt8 value) override;
		IStream& operator<<(UInt16 value) override;
		IStream& operator<<(UInt32 value) override;
		IStream& operator<<(UInt64 value) override;

	protected:
		SizeT GetAccessible();

	private:
		typedef std::unique_ptr<Buffer> BufferPtr;
		typedef std::vector<BufferPtr>  Chunks;

		File   m_file;
		UInt64 m_fileSize;
		UInt32 m_chunkSize;
		SizeT  m_position;
		SizeT  m_chunkBegin;
		SizeT  m_chunkEnd;
		SizeT  m_currentChunk;
		Chunks m_chunks;

	};
}
