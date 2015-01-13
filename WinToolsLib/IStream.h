#pragma once
#include "Types.h"
#include <memory>

namespace WinToolsLib
{
	class IStream
	{
	public:
		virtual ~IStream() {}

		virtual SizeT GetPosition() const = 0;
		virtual Void SetPosition(SizeT position) = 0;
		
		virtual Void Reset() = 0;
		virtual Void Skip(SizeT bytes) = 0;

		virtual Void Read(PByte buffer, SizeT bytes) = 0;
		virtual Void Write(PByte buffer, SizeT bytes) = 0;

		virtual IStream& operator>>(Int8& value) = 0;
		virtual IStream& operator>>(Int16& value) = 0;
		virtual IStream& operator>>(Int32& value) = 0;
		virtual IStream& operator>>(Int64& value) = 0;

		virtual IStream& operator>>(UInt8& value) = 0;
		virtual IStream& operator>>(UInt16& value) = 0;
		virtual IStream& operator>>(UInt32& value) = 0;
		virtual IStream& operator>>(UInt64& value) = 0;

		virtual IStream& operator<<(Int8 value) = 0;
		virtual IStream& operator<<(Int16 value) = 0;
		virtual IStream& operator<<(Int32 value) = 0;
		virtual IStream& operator<<(Int64 value) = 0;

		virtual IStream& operator<<(UInt8 value) = 0;
		virtual IStream& operator<<(UInt16 value) = 0;
		virtual IStream& operator<<(UInt32 value) = 0;
		virtual IStream& operator<<(UInt64 value) = 0;
	};

	typedef std::unique_ptr<IStream>      IStreamPtr;
	typedef std::tr1::weak_ptr<IStream>   IStreamWeakPtr;
	typedef std::tr1::shared_ptr<IStream> IStreamSharedPtr;
}
