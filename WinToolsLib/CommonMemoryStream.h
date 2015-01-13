#pragma once
#include "Types.h"
#include "IStream.h"
#include "Handles\Handle.h"

namespace WinToolsLib
{
	class CommonMemoryStream : public IStream
	{
	public:
		SizeT GetPosition() const override;
		Void SetPosition(SizeT position) override;
		
		Void Reset() override;
		Void Skip(SizeT bytes) override;

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
		SizeT  m_position;
	};
}

