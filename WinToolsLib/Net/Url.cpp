#include "Url.h"
#include "..\Exception.h"

#include <vector>

#include <Windows.h>
#include <WinInet.h>
#include <Shlwapi.h>
#pragma comment (lib, "Shlwapi.lib")

namespace WinToolsLib
{
	namespace Net
	{
		String Url::Encode(const TChar* url, EncodePortion flag)
		{
			const auto maxAttempts = 4;
			const auto maxLength = INTERNET_MAX_URL_LENGTH;
			const auto step = maxLength / maxAttempts;
			
			for (auto i = 1; i <= maxAttempts; i++)
			{
				DWORD length = step * i;
				std::vector<TChar> buffer(length + 1);

				auto result = ::UrlEscape(url, &buffer[0], &length, static_cast<UInt32>(flag));
				if (S_OK == result)
				{
					return String(&buffer[0]);
				}
				if (E_POINTER == result)
				{
					continue;
				}
				THROW_WIN32_EXCEPTION(result);
			}
			THROW_WIN32_EXCEPTION(E_POINTER);
		}

		String Url::Decode(const TChar* url)
		{
			const auto maxAttempts = 4;
			const auto maxLength = INTERNET_MAX_URL_LENGTH;
			const auto step = maxLength / maxAttempts;
			
			for (auto i = 1; i <= maxAttempts; i++)
			{
				DWORD length = step * i;
				std::vector<TChar> buffer(length + 1);

				auto result = ::UrlUnescape((PTSTR)url, &buffer[0], &length, 0);
				if (S_OK == result)
				{
					return String(&buffer[0]);
				}
				if (E_POINTER == result)
				{
					continue;
				}
				THROW_WIN32_EXCEPTION(result);
			}
			THROW_WIN32_EXCEPTION(E_POINTER);
		}
	}
}
