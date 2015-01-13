#pragma once
#include "..\Types.h"
#include "..\Buffer.h"
#include "..\FileSystem\File.h"

namespace WinToolsLib { namespace Cryptography
{
	class SymmetricCipher
	{
	public:
		Void Compute(const Byte* data, SizeT size);
		Void Compute(const Buffer& buffer);
		Void Compute(const StringA& string);
		Void Compute(const StringW& string);
		Void Compute(const FileSystem::File& file, SizeT chunkSize = 16384);

		virtual Buffer GetOutput();
		virtual Void Reset() = 0;

	protected:
		virtual Void Update(PCByte data, SizeT size) = 0;
		virtual Void Finalize() = 0;

	protected:
		Buffer m_out;
	};

	inline Buffer SymmetricCipher::GetOutput()
	{
		return std::move(m_out);
	}
} }
