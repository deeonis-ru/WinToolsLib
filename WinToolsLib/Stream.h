#pragma once
#include "Types.h"
#include "IStream.h"
#include "Buffer.h"

namespace WinToolsLib
{
	class Stream : public IStream
	{
	public:
		Stream(Buffer& buffer);
		Stream(Buffer& buffer, SizeT offset);
		
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

	private:
		Buffer& m_buffer;
		SizeT m_position;
	};
}
