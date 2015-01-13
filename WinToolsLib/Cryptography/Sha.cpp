#include "Sha.h"
#include "..\Exception.h"
#include <assert.h>

#pragma intrinsic(_byteswap_ulong)

//
// Based on Paul E. Jones SHA1 implementation
//
// Copyright (C) 1998, 2009
// Paul E. Jones <paulej@packetizer.com>
// All Rights Reserved
//
// Description:
//     This file implements the Secure Hashing Standard as defined
//     in FIPS PUB 180-1 published April 17, 1995.
//
//     The Secure Hashing Standard, which uses the Secure Hashing
//     Algorithm (SHA), produces a 160-bit message digest for a
//     given data stream.  In theory, it is highly improbable that
//     two messages will produce the same message digest.  Therefore,
//     this algorithm can serve as a means of providing a "fingerprint"
//     for a message.
//
// Portability Issues:
//     SHA-1 is defined in terms of 32-bit "words".  This code was
//     written with the expectation that the processor has at least
//     a 32-bit machine word size.  If the machine word size is larger,
//     the code should still function properly.  One caveat to that
//     is that the input functions taking characters and character
//     arrays assume that only 8 bits of information are stored in each
//     character.
//
// Caveats:
//     SHA-1 is designed to work with messages less than 2^64 bits
//     long. Although SHA-1 allows a message digest to be generated for
//     messages of any number of bits less than 2^64, this
//     implementation only works with messages with a length that is a
//     multiple of the size of an 8-bit character.
//

template <class T, class B> inline T CircularShift(B bits, T word)
{
	assert(bits < sizeof(T)*8);
	return T((word<<bits) | (word>>(sizeof(T)*8-bits)));
}

namespace WinToolsLib
{
	namespace Cryptography
	{
		Sha1::Sha1()
		{
			Reset();
		}

		Void Sha1::Update(const Byte* data, SizeT size)
		{
			if (nullptr == data || 0 == size)
			{
				return;
			}

			if (m_isComputed || m_isCorrupted)
			{
				m_isCorrupted = True;
				return;
			}

			while (size-- && !m_isCorrupted)
			{
				m_block[m_index++] = (*data & 0xFF);

				m_lengthLow += 8;
				if (0 == m_lengthLow)
				{
					m_lengthHigh++;
					if (0 == m_lengthHigh)
					{
						m_isCorrupted = True;
					}
				}

				if (64 == m_index)
				{
					ProcessMessageBlock();
				}

				data++;
			}
		}

		Void Sha1::Finalize()
		{
			if (m_isCorrupted)
			{
				return;
			}

			if (!m_isComputed)
			{
				PadMessage();
				m_isComputed = True;
			}
		}

		Void Sha1::Reset()
		{
			m_digest[0] = 0x67452301L;
			m_digest[1] = 0xEFCDAB89L;
			m_digest[2] = 0x98BADCFEL;
			m_digest[3] = 0x10325476L;
			m_digest[4] = 0xC3D2E1F0L;

			m_lengthLow = 0;
			m_lengthHigh = 0;
			m_index = 0;
			m_isComputed = False;
			m_isCorrupted = False;
		}

		Buffer Sha1::GetDigest() const
		{
			if (m_isCorrupted || !m_isComputed)
			{
				return Buffer();
			}

			Buffer digest(20);
			UInt32* buffer = (UInt32*)digest.GetBuffer();
				
			for (auto i = 0; i < 5; i++)
			{
				*buffer++ = _byteswap_ulong(m_digest[i]);
			}

			return digest;
		}

		//
		// This function will process the next 512 bits of the message
		// stored in the m_block array.
		//
		Void Sha1::ProcessMessageBlock()
		{
			// Constants defined in SHA-1
			const UInt32 k[] =
			{
				0x5A827999,
				0x6ED9EBA1,
				0x8F1BBCDC,
				0xCA62C1D6
			};

			// Word sequence
			UInt32 w[80] = {0};
			
			// Initialize the first 16 words in the array w
			for (auto t = 0; t < 16; t++)
			{
				w[t] = ((UInt32)m_block[t * 4]) << 24;
				w[t] |= ((UInt32)m_block[t * 4 + 1]) << 16;
				w[t] |= ((UInt32)m_block[t * 4 + 2]) << 8;
				w[t] |= ((UInt32)m_block[t * 4 + 3]);
			}

			for (auto t = 16; t < 80; t++)
			{
				w[t] = CircularShift(1, w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16]);
			}

			auto a = m_digest[0];
			auto b = m_digest[1];
			auto c = m_digest[2];
			auto d = m_digest[3];
			auto e = m_digest[4];

			for (auto t = 0; t < 20; t++)
			{
				UInt32 temp = CircularShift(5, a) + ((b & c) | ((~b) & d)) + e + w[t] + k[0];
				temp &= 0xFFFFFFFF;
				e = d;
				d = c;
				c = CircularShift(30, b);
				b = a;
				a = temp;
			}

			for (auto t = 20; t < 40; t++)
			{
				UInt32 temp = CircularShift(5, a) + (b ^ c ^ d) + e + w[t] + k[1];
				temp &= 0xFFFFFFFF;
				e = d;
				d = c;
				c = CircularShift(30, b);
				b = a;
				a = temp;
			}

			for (auto t = 40; t < 60; t++)
			{
				UInt32 temp = CircularShift(5, a) + ((b & c) | (b & d) | (c & d)) + e + w[t] + k[2];
				temp &= 0xFFFFFFFF;
				e = d;
				d = c;
				c = CircularShift(30, b);
				b = a;
				a = temp;
			}

			for (auto t = 60; t < 80; t++)
			{
				UInt32 temp = CircularShift(5, a) + (b ^ c ^ d) + e + w[t] + k[3];
				temp &= 0xFFFFFFFF;
				e = d;
				d = c;
				c = CircularShift(30, b);
				b = a;
				a = temp;
			}

			m_digest[0] = m_digest[0] + a;
			m_digest[1] = m_digest[1] + b;
			m_digest[2] = m_digest[2] + c;
			m_digest[3] = m_digest[3] + d;
			m_digest[4] = m_digest[4] + e;

			m_index = 0;
		}

		//
		// According to the standard, the message must be padded to an even
		// 512 bits.  The first padding bit must be a '1'.  The last 64
		// bits represent the length of the original message.  All bits in
		// between should be 0.  This function will pad the message
		// according to those rules by filling the Message_Block array
		// accordingly.  It will also call SHA1ProcessMessageBlock()
		// appropriately.  When it returns, it can be assumed that the
		// message digest has been computed.
		//
		Void Sha1::PadMessage()
		{
			// Check to see if the current message block is too small to hold
			// the initial padding bits and length.  If so, we will pad the
			// block, process it, and then continue padding into a second
			// block.
			if (m_index > 55)
			{
				m_block[m_index++] = 0x80;

				while (m_index < 64)
				{
					m_block[m_index++] = 0;
				}

				ProcessMessageBlock();

				while (m_index < 56)
				{
					m_block[m_index++] = 0;
				}
			}
			else
			{
				m_block[m_index++] = 0x80;

				while (m_index < 56)
				{
					m_block[m_index++] = 0;
				}
			}

			// Store the message length as the last 8 octets
			m_block[56] = (m_lengthHigh >> 24) & 0xFF;
			m_block[57] = (m_lengthHigh >> 16) & 0xFF;
			m_block[58] = (m_lengthHigh >> 8) & 0xFF;
			m_block[59] = (m_lengthHigh) & 0xFF;
			m_block[60] = (m_lengthLow >> 24) & 0xFF;
			m_block[61] = (m_lengthLow >> 16) & 0xFF;
			m_block[62] = (m_lengthLow >> 8) & 0xFF;
			m_block[63] = (m_lengthLow) & 0xFF;

			ProcessMessageBlock();
		}
	}
}
