#pragma once
#include "Types.h"
#include "Stream.h"
#include "Buffer.h"

namespace WinToolsLib
{
	class BufferStream : public Buffer, public Stream
	{
	public:
		BufferStream(Buffer&& buffer);
	};
}
