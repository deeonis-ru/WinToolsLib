#include "BufferStream.h"

#pragma warning(disable:4355)

namespace WinToolsLib
{
	BufferStream::BufferStream(Buffer&& buffer) :
		Buffer(std::move(buffer)),
		Stream((Buffer&)*this)
	{
	}
}