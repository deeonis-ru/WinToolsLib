#pragma once
#include "..\Types.h"
#include "..\Buffer.h"
#include "Hash.h"

namespace WinToolsLib
{
	namespace Cryptography
	{
		class Sha1 : public Hash
		{
		public:
			Sha1();

			Void Update(const Byte* data, SizeT size) override;
			Void Finalize() override;
			Void Reset() override;

			Buffer GetDigest() const override;

		private:
			Void ProcessMessageBlock();
			Void PadMessage();

		private:
			UInt32 m_digest[5];
			UInt32 m_lengthLow;
			UInt32 m_lengthHigh;

			Byte   m_block[64];
			UInt32 m_index;

			Bool m_isComputed;
			Bool m_isCorrupted;
		};
	}
}
