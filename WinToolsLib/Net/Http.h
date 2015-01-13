#pragma once
#include "..\Types.h"
#include "..\Buffer.h"
#include "..\Handles\InternetHandle.h"

namespace WinToolsLib
{
	namespace Net
	{
		using namespace Handles;

		struct Flag
		{
			enum
			{
				Reload = INTERNET_FLAG_RELOAD,
				Secure = INTERNET_FLAG_SECURE,
				NoCacheWrite = INTERNET_FLAG_NO_CACHE_WRITE,
				IgnoreCertInvalidName = INTERNET_FLAG_IGNORE_CERT_CN_INVALID,
				IgnoreCertInvalidDate = INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
			};
		};

		class Http
		{
		public:
			Http(PCTChar userAgent);
			Http(Http&& other);

			Http& operator=(Http&& other);

			Void Connect(PCTChar pszServer, UInt16 port);

			static Bool IsOffline(InternetHandle hInternet = nullptr);
			static Void GoOnline(InternetHandle hInternet = nullptr);

			Void SendRequest(
				PCTChar pszVerb,
				PCTChar pszUrl,
				PCTChar pszHeaders,
				PCByte pBody,
				UInt32 bodyLength,
				UInt32 flags = 0);

			Void SendGetRequest(
				PCTChar pszUrl,
				UInt32 flags = 0);

			Void SendPostRequest(
				PCTChar pszUrl,
				PCTChar pszHeaders,
				PCByte pBody,
				UInt32 bodyLength,
				UInt32 flags = 0);

			UInt32 GetContentSize() const;

			typedef std::function<Void(UInt32 bytesRead)> ReadFileCallback;
			Buffer ReadFile(UInt32 chunkSize = (256 * 1024), ReadFileCallback callback = nullptr) const;
			
			StringA ReadStringA(UInt32 chunkSize = 1024) const;

			struct Options
			{
				enum
				{
					TryToGoOnline = 1,
					IgnoreUnknownCertificateAuthority
				};
			};
			Void SetOption(UInt32 option);
			Void SetSendTimeout(UInt32 milliseconds);

		protected:
			Void MoveFrom(Http& other);
			Void SetIgnoreUnknownCertificateAuthority();
			Void SetSendTimeout();

		private:
			InternetHandle m_hInternet;
			InternetHandle m_hConnect;
			InternetHandle m_hRequest;
			UInt32 m_options;
			UInt32 m_sendTimeout;
		};
		
		inline Void Http::SetOption(UInt32 option)
		{
			m_options |= option;
		}

		inline Void Http::SetSendTimeout(UInt32 milliseconds)
		{
			m_sendTimeout = milliseconds;
		}
	}
}
