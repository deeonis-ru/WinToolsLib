#pragma once
#include "..\Types.h"
#include "IHash.h"

namespace WinToolsLib
{
	namespace Cryptography
	{
		class Hash : public IHash
		{
		public:
			Void ComputeHash(const Byte* data, SizeT size) override;
			Void ComputeHash(const Buffer& buffer) override;
			Void ComputeHash(const StringA& string) override;
			Void ComputeHash(const StringW& string) override;
			Void ComputeHash(const FileSystem::File& file, SizeT chunkSize = 16384) override;
		};
	}
}
