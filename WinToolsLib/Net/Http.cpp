#include "Http.h"
#include "..\Exception.h"
#include <string.h>
#include <tchar.h>
#include <Windows.h>
#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

namespace WinToolsLib
{
	namespace Net
	{
		Http::Http(PCTChar userAgent) :
			m_options(Options::TryToGoOnline),
			m_sendTimeout(0)
		{
			m_hInternet = ::InternetOpen(
				userAgent,
				INTERNET_OPEN_TYPE_PRECONFIG,
				NULL,
				NULL,
				0);

			if (m_hInternet.IsNull())
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		Http::Http(Http&& other)
		{
			MoveFrom(other);
		}

		Http& Http::operator=(Http&& other)
		{
			MoveFrom(other);
			return *this;
		}

		Void Http::Connect(PCTChar pszServer, UInt16 port)
		{
			m_hConnect = ::InternetConnect(
				m_hInternet,
				pszServer,
				port,
				NULL,
				NULL,
				INTERNET_SERVICE_HTTP,
				0,
				NULL);

			if (m_hConnect.IsNull())
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		Bool Http::IsOffline(InternetHandle hInternet)
		{
			INTERNET_CONNECTED_INFO info = {0};
			DWORD sizeInBytes = sizeof(info);

			auto success = ::InternetQueryOption(
				hInternet,
				INTERNET_OPTION_CONNECTED_STATE,
				&info,
				&sizeInBytes);

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			if (info.dwConnectedState & INTERNET_STATE_DISCONNECTED_BY_USER)
			{
				return True;
			}
			return False;
		}

		Void Http::GoOnline(InternetHandle hInternet)
		{
			INTERNET_CONNECTED_INFO info = {0};
			DWORD sizeInBytes = sizeof(info);

			info.dwConnectedState = INTERNET_STATE_CONNECTED;

			auto success = ::InternetSetOption(
				hInternet,
				INTERNET_OPTION_CONNECTED_STATE,
				&info,
				sizeInBytes);

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		Void Http::SendRequest(
			PCTChar pszVerb,
			PCTChar pszUrl,
			PCTChar pszHeaders,
			PCByte pBody,
			UInt32 bodyLength,
			UInt32 flags)
		{
			m_hRequest = ::HttpOpenRequest(
				m_hConnect,
				pszVerb,
				pszUrl,
				NULL,
				NULL,
				NULL,
				flags,
				NULL);

			if (m_hRequest.IsNull())
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			SetSendTimeout();

			auto attempt = 0;
			while (True)
			{
				auto success = ::HttpSendRequest(
					m_hRequest,
					pszHeaders,
					pszHeaders ? (DWORD)_tcslen(pszHeaders) : 0,
					(LPVOID)pBody,
					bodyLength);

				if (success)
				{
					return;
				}
				
				auto lastError = ::GetLastError();
				if (attempt++ < 4)
				{
					if (ERROR_FILE_NOT_FOUND == lastError)
					{
						if (m_options & Options::TryToGoOnline)
						{
							GoOnline();
							continue;
						}
					}
					else if (ERROR_INTERNET_INVALID_CA == lastError)
					{
						if (m_options & Options::IgnoreUnknownCertificateAuthority)
						{
							SetIgnoreUnknownCertificateAuthority();
							continue;
						}
					}
				}

				THROW_WIN32_EXCEPTION(lastError);
			}
		}

		Void Http::SendGetRequest(
			PCTChar pszUrl,
			UInt32 flags)
		{
			SendRequest(Text("GET"), pszUrl, NULL, NULL, 0, flags);
		}

		Void Http::SendPostRequest(
			PCTChar pszUrl,
			PCTChar pszHeaders,
			PCByte pBody,
			UInt32 bodyLength,
			UInt32 flags)
		{
			SendRequest(Text("POST"), pszUrl, pszHeaders, pBody, bodyLength, flags);
		}

		UInt32 Http::GetContentSize() const
		{
			if (!m_hRequest)
			{
				return 0;
			}

			UInt32 contentSize = 0;
			auto bufferSize = sizeof(contentSize);

			auto success = ::HttpQueryInfo(
				m_hRequest,
				HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
				&contentSize,
				(LPDWORD)&bufferSize,
				NULL);
			
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
			return contentSize;
		}

		Buffer Http::ReadFile(UInt32 chunkSize, ReadFileCallback callback) const
		{
			Buffer file;
			Buffer chunk(chunkSize);

			DWORD bytesRead = 0;
			while (True)
			{
				auto success = ::InternetReadFile(
					m_hRequest,
					chunk.GetBuffer(),
					(DWORD)chunk.GetSize(),
					&bytesRead);

				if (!success)
				{
					THROW_LAST_ERROR_EXCEPTION();
				}

				if (0 == bytesRead)
				{
					break;
				}

				file.Append(chunk, bytesRead);
				
				if (callback)
				{
					callback(static_cast<UInt32>(file.GetSize()));
				}
			}

			return file;
		}

		StringA Http::ReadStringA(UInt32 chunkSize) const
		{
			StringA string;
			StringA chunk;
			chunk.Resize(chunkSize);
			
			DWORD bytesRead = 0;
			while (True)
			{
				auto success = ::InternetReadFile(
					m_hRequest,
					chunk.GetBuffer(),
					(DWORD)chunk.GetSize(),
					&bytesRead);

				if (!success)
				{
					THROW_LAST_ERROR_EXCEPTION();
				}

				if (0 == bytesRead)
				{
					break;
				}

				string.Append(chunk, bytesRead);
			}

			return string;
		}

		Void Http::SetIgnoreUnknownCertificateAuthority()
		{
			DWORD flags = 0;
			DWORD sizeInBytes = sizeof(flags);

			auto success = ::InternetQueryOption(
				m_hRequest,
				INTERNET_OPTION_SECURITY_FLAGS,
				&flags,
				&sizeInBytes);

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			flags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;

			success = ::InternetSetOption(
				m_hRequest,
				INTERNET_OPTION_SECURITY_FLAGS,
				&flags,
				sizeInBytes);

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		Void Http::SetSendTimeout()
		{
			if (0 == m_sendTimeout)
			{
				return;
			}

			auto success = ::InternetSetOption(
				m_hRequest,
				INTERNET_OPTION_SEND_TIMEOUT,
				&m_sendTimeout,
				sizeof(m_sendTimeout));

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		Void Http::MoveFrom(Http& other)
		{
			if (this != &other)
			{
				m_hInternet = std::move(other.m_hInternet);
				m_hConnect = std::move(other.m_hConnect);
				m_hRequest = std::move(other.m_hRequest);
				
				m_options = other.m_options;
				m_sendTimeout = other.m_sendTimeout;

				other.m_options = 0;
				other.m_sendTimeout = 0;
			}
		}
	}
}
