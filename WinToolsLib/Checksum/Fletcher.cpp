#include "Fletcher.h"

namespace WinToolsLib { namespace Checksum
{
	UInt16 Fletcher16::Calc(PCByte data, SizeT size)
	{
		UInt16 sum1 = 0xff;
		UInt16 sum2 = 0xff;
 
		while (size)
		{
			SizeT tlen = size > 20 ? 20 : size;
            size -= tlen;
            do 
			{
				sum1 += *data++;
				sum2 += sum1;
			} 
			while (--tlen);
			sum1 = (sum1 & 0xff) + (sum1 >> 8);
			sum2 = (sum2 & 0xff) + (sum2 >> 8);
		}
			
		// Second reduction step to reduce sums to 8 bits 
		sum1 = (sum1 & 0xff) + (sum1 >> 8);
		sum2 = (sum2 & 0xff) + (sum2 >> 8);
		return sum2 << 8 | sum1;
	}

} }