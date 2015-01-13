#include "Mac.h"
#include "..\Exception.h"

#include <string.h>
#include <tchar.h>

#include <WinSock2.h>
#include <IPHlpApi.h>
#pragma comment (lib, "IPHlpApi.lib")

namespace WinToolsLib
{
	namespace Net
	{
		Buffer Mac::GetFirst()
		{
			//
			// The recommended method of calling the GetAdaptersAddresses function
			// is to pre-allocate a 15KB working buffer pointed to by the AdapterAddresses parameter.
			//
			ULONG size = 15 * 1024;
			while (True)
			{
				Buffer buffer(size);
				PIP_ADAPTER_ADDRESSES pAddresses = (PIP_ADAPTER_ADDRESSES)buffer.GetBuffer();
				
				auto result = ::GetAdaptersAddresses(0, 0, NULL, pAddresses, &size);
				if (ERROR_BUFFER_OVERFLOW == result)
				{
					continue;
				}
				else if (ERROR_SUCCESS == result)
				{
					Buffer mac(pAddresses->PhysicalAddress, pAddresses->PhysicalAddressLength);
					return mac;
				}

				break;
			}

			return Buffer();
		}
	}
}
