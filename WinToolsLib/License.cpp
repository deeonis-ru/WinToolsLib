#include "License.h"
#include "Exception.h"
#include "Registry\Key.h"
#include "Checksum\Crc.h"
#include "Process\Process.h"

namespace WinToolsLib
{
	String License::GetUserId()
	{
		auto processorId = GetProcessorId();
		auto productId = GetProductId();
		auto userId = String::FormatS(Text("%s/%s"), processorId.GetBuffer(), productId.GetBuffer());

		auto data = (PCByte)userId.GetBuffer();
		auto size = userId.GetLength() * sizeof(TChar);
		auto hash = Checksum::Crc32::Calc(data, size);
		
		return String::FormatS(Text("%010u"), hash);
	}

	String License::GetProcessorId()
	{
		struct
		{
			UInt32 eax;
			UInt32 ebx;
			UInt32 ecx;
			UInt32 edx;
		} id;

		__cpuid((Int32*)&id, 1);

		return String::FormatS(Text("%08X%08X"), id.edx, id.eax);
	}

	String License::GetProductId()
	{
		try
		{
			auto currentVersion = Registry::Key::Open(
				HKEY_LOCAL_MACHINE,
				Text("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),
				Process::Process::IsWow64() ? KEY_WOW64_64KEY | KEY_QUERY_VALUE : KEY_QUERY_VALUE);

			const String keyName(Text("ProductId"));
			String productId;

			currentVersion.EnumValue([&keyName, &productId](const TChar* name, UInt32 type, Buffer& data)
			{
				if (keyName == name && REG_SZ == type)
				{
					productId = (const TChar*)data.GetBuffer();
				}
			});

			return productId;
		}
		catch (Exception&)
		{
		}

		return String();
	}
}