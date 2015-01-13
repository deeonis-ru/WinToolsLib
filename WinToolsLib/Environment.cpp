#include "Environment.h"
#include "Exception.h"
#include "Buffer.h"
#include <Windows.h>
#include <Userenv.h>
#pragma comment(lib, "Userenv.lib")

namespace WinToolsLib
{
	String Environment::ExpandString(const TChar* pszString, const Handle& hToken)
	{
		UInt32 nAttempts = 6;
		UInt32 sizeInChars = 512;

		// Lets start from 512 chars buffer and increase it up to 32768 chars if necessary.
		while (nAttempts > 0)
		{
			UInt32 sizeInBytes = sizeInChars * sizeof(TChar);
			Buffer buffer(sizeInBytes);

			auto success = ::ExpandEnvironmentStringsForUser(hToken, pszString, (PTChar)buffer.GetBuffer(), sizeInChars - 1);
			if (success)
			{
				// TODO: Use String::GetBuffer(sizeInChars) when it is ready.
				return String((PTChar)buffer.GetBuffer());
			}

			auto lastError = ::GetLastError();
			if (lastError != ERROR_INSUFFICIENT_BUFFER)
			{
				THROW_WIN32_EXCEPTION(lastError);
			}
			
			sizeInChars *= 2;
			nAttempts--;
		}

		// We failed to expand string, lets just return the original one.
		return String(pszString);
	}

	String Environment::GetVariable(const TChar* name)
	{
		UInt32 nAttempts = 6;
		UInt32 sizeInChars = 512;

		// Lets start from 512 chars buffer and increase it up to 32768 chars if necessary.
		while (nAttempts > 0)
		{
			UInt32 sizeInBytes = sizeInChars * sizeof(TChar);
			Buffer buffer(sizeInBytes);

			auto nChars = ::GetEnvironmentVariable(name, (PTChar)buffer.GetBuffer(), sizeInChars - 1);
			if (nChars > 0)
			{
				// TODO: Use String::GetBuffer(sizeInChars) when it is ready.
				return String((PTChar)buffer.GetBuffer());
			}

			auto lastError = ::GetLastError();
			if (lastError != ERROR_INSUFFICIENT_BUFFER)
			{
				THROW_WIN32_EXCEPTION(lastError);
			}
			
			sizeInChars *= 2;
			nAttempts--;
		}

		// We failed to expand string, lets just return the original one.
		return String(name);
	}
}