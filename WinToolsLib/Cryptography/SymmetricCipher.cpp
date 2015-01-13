#pragma once
#include "SymmetricCipher.h"

namespace WinToolsLib { namespace Cryptography
{
	Void SymmetricCipher::Compute(const Byte* data, SizeT size)
	{
		Update(data, size);
		Finalize();
	}

	Void SymmetricCipher::Compute(const Buffer& buffer)
	{
		Compute(buffer.GetBuffer(), buffer.GetSize());
	}

	Void SymmetricCipher::Compute(const StringA& string)
	{
		Compute((PCByte)string.GetBuffer(), string.GetSize() * sizeof(Char));
	}

	Void SymmetricCipher::Compute(const StringW& string)
	{
		Compute((PCByte)string.GetBuffer(), string.GetSize() * sizeof(WChar));
	}

	Void SymmetricCipher::Compute(const FileSystem::File& file, SizeT chunkSize)
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
} }
