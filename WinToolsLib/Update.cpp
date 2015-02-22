#include "Update.h"
#include "Exception.h"
#include "License.h"
#include "System.h"
#include "Process.h"

namespace WinToolsLib 
{
	Update::Update()
	{
		m_parser = [](const String& response, UpdateResponseElement elem){
			return Text("");
		};
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
		const TChar* request,
		const ResponseParser& parser)
	{
		auto http = CreateSession(server, port, options, userAgent);
		const auto& fullRequest = ConstructRequest(appVersion, request);

		http.SendGetRequest(fullRequest, flags);
		auto response = http.ReadStringA();

		Update update;
		update.m_server = server;
		update.m_port = port;
		update.m_flags = flags;
		update.m_options = options;
		update.m_userAgent = userAgent;
		update.m_appVersion = appVersion;
		update.m_parser = parser;
		update.m_response = String::Convert(response);
		return update;
	}

	Buffer Update::Download() const
	{
		auto attempt = 0;
		for (;;)
		{
			try
			{
				auto path = m_parser(m_response, UpdateResponseElement::DownloadPath);
				auto http = CreateSession(m_server, m_port, m_options, m_userAgent);
				http.SendGetRequest(path, m_flags);

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
		auto path = m_parser(m_response, UpdateResponseElement::DownloadPath);
		auto http = CreateSession(m_server, m_port, m_options, m_userAgent);
		http.SendGetRequest(path, m_flags);

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
		const TChar* req)
	{
		static String request = [=]()
		{
			auto request = String::FormatS(
				req,
				appVersion);

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
			m_response = std::move(other.m_response);
			m_parser = std::move(other.m_parser);

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
			m_response = other.m_response;
			m_parser = other.m_parser;
		}
	}
}