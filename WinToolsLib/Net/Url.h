#pragma once
#include "..\Types.h"
#include "..\String.h"

namespace WinToolsLib
{
	namespace Net
	{
		enum class EncodePortion : UInt32
		{
			None        = 0x00000000,
			SegmentOnly = 0x00002000
		};

		class Url
		{
		public:
			static String Encode(const TChar* url, EncodePortion flag = EncodePortion::None);
			static String Decode(const TChar* url);
		};
	}
}
