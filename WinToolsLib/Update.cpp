#include "Update.h"
#include "Exception.h"
#include "License.h"
#include "System.h"
#include "Process.h"

namespace WinToolsLib 
{
	Update::Update()
	{
	}

	Update::Update(Update&& other)
	{
		MoveFrom(other);
	}

	Update::Update(const Update& other)
	{
		CopyFrom(other);
	}

	Update& Update::operator=(Update&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Update& Update::operator=(const Update& other)
	{
		CopyFrom(other);
		return *this;
	}

	Update Update::Check(
		const TChar* server,
		const UInt16 port,
		const UInt32 flags,
		const UInt32 options,
		const TChar* userAgent,
		const TChar* appVersion,
		const TChar* additionalParams)
	{
		auto http = CreateSession(server, port, options, userAgent);
		const auto& request = ConstructRequest(appVersion, additionalParams);

		http.SendGetRequest(request, flags);
		auto response = http.ReadStringA();

		Update update;
		update.m_server = server;
		update.m_port = port;
		update.m_flags = flags;
		update.m_options = options;
		update.m_userAgent = userAgent;
		update.m_appVersion = appVersion;
		update.m_path = String::Convert(response);
		return update;
	}

	Buffer Update::Download() const
	{
		auto attempt = 0;
		for (;;)
		{
			try
			{
				auto http = CreateSession(m_server, m_port, m_options, m_userAgent);
				http.SendGetRequest(m_path, m_flags);

				auto totalBytes = http.GetContentSize();
				auto file = http.ReadFile();
				if (file.GetSize() < totalBytes)
				{
					THROW_WIN32_EXCEPTION(ERROR_INCORRECT_SIZE);
				}
				return file;
			}
			catch (...)
			{
				if (attempt++ < 3)
				{
					continue;
				}
				else
				{
					throw;
				}
			}
		}
	}

	Buffer Update::Download(UInt32 chunks, DownloadCallback callback) const
	{
		auto http = CreateSession(m_server, m_port, m_options, m_userAgent);
		http.SendGetRequest(m_path, m_flags);

		auto totalBytes = http.GetContentSize();
		auto readCallback = [=](UInt32 bytesRead)
		{
			callback(totalBytes, bytesRead);
		};

		return http.ReadFile(totalBytes / chunks, readCallback);
	}

	Net::Http Update::CreateSession(
		const TChar* server,
		const UInt16 port,
		const UInt32 options,
		const TChar* userAgent)
	{
		Net::Http http(userAgent);
		http.SetOption(options);
		http.Connect(server, port);
		return http;
	}

	const String& Update::ConstructRequest(
		const TChar* appVersion,
		const TChar* additionalParams)
	{
		static String request = [=]()
		{
			const auto uid = License::GetUserId();
			const auto os = System::GetFullVersion();

#ifndef _WIN64
			const auto bits = Process::IsWow64() ? Text("64") : Text("32");
#else
			const auto bits = Text("64");
#endif
			auto request = String::FormatS(
				L"/update.php?uid=%s&app=%s&os=%s&bits=%s",
				uid.GetBuffer(),
				appVersion,
				os.GetBuffer(),
				bits);

			if (additionalParams != nullptr)
			{
				request += additionalParams;
			}
			return request;
		}();

		return request;
	}

	Void Update::MoveFrom(Update& other)
	{
		if (this != &other)
		{
			m_server = std::move(other.m_server);
			m_port = other.m_port;
			m_flags = other.m_flags;
			m_options = other.m_options;
			m_userAgent = std::move(other.m_userAgent);
			m_appVersion = std::move(other.m_appVersion);
			m_path = std::move(other.m_path);

			other.m_port = 0;
			other.m_flags = 0;
			other.m_options = 0;
		}
	}

	Void Update::CopyFrom(const Update& other)
	{
		if (this != &other)
		{
			m_server = other.m_server;
			m_port = other.m_port;
			m_flags = other.m_flags;
			m_options = other.m_options;
			m_userAgent = other.m_userAgent;
			m_appVersion = other.m_appVersion;
			m_path = other.m_path;
		}
	}
}