#pragma once
#include "..\Types.h"
#include "..\Buffer.h"
#include "..\String.h"
#include "..\FileSystem\File.h"

namespace WinToolsLib
{
	namespace Cryptography
	{
		class IHash
		{
		public:
			virtual ~IHash() {}

			virtual Void ComputeHash(const Byte* data, SizeT size) = 0;
			virtual Void ComputeHash(const Buffer& buffer) = 0;
			virtual Void ComputeHash(const StringA& string) = 0;
			virtual Void ComputeHash(const StringW& string) = 0;
			virtual Void ComputeHash(const FileSystem::File& file, SizeT chunkSize = 0) = 0;

			virtual Void Update(const Byte* data, SizeT size) = 0;
			virtual Void Finalize() = 0;
			virtual Void Reset() = 0;

			virtual Buffer GetDigest() const = 0;
		};
	}
}
