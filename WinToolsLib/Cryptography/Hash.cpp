#include "Hash.h"
#include "..\Exception.h"

namespace WinToolsLib
{
	namespace Cryptography
	{
		Void Hash::ComputeHash(const Byte* data, SizeT size)
		{
			Update(data, size);
			Finalize();
		}

		Void Hash::ComputeHash(const Buffer& buffer)
		{
			ComputeHash(buffer.GetBuffer(), buffer.GetSize());
		}

		Void Hash::ComputeHash(const StringA& string)
		{
			ComputeHash((PCByte)string.GetBuffer(), string.GetSize() * sizeof(Char));
		}

		Void Hash::ComputeHash(const StringW& string)
		{
			ComputeHash((PCByte)string.GetBuffer(), string.GetSize() * sizeof(WChar));
		}

		Void Hash::ComputeHash(const FileSystem::File& file, SizeT chunkSize)
		{
			auto data = Buffer(chunkSize);
			auto bytes = file.Read(data);
			while (bytes > 0)
			{
				Update(data.GetBuffer(), bytes);
				bytes = file.Read(data);
			}
			Finalize();
		}
	}
}
