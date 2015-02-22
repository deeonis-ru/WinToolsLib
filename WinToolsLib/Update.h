#pragma once
#include "Types.h"
#include "String.h"
#include "Buffer.h"
#include "Version.h"
#include "Net\Http.h"

namespace WinToolsLib
{
	enum class UpdateResponseElement
	{
		DownloadUrl,
		DownloadPath,
		AppName,
		AppVersion
	};

	class Update
	{
		Update();

	public:
		Update(Update&& other);
		Update(const Update& other);

		Update& operator=(Update&& other);
		Update& operator=(const Update& other);

		typedef std::function<String(const String& response, UpdateResponseElement elem)> ResponseParser;
		static Update Check(
			const TChar* server,
			const UInt16 port,
			const UInt32 flags,
			const UInt32 options,
			const TChar* userAgent,
			const TChar* appVersion,
			const TChar* request,
			const ResponseParser& parser);

		Bool IsAvailable() const;
		Version GetLatestVersion() const;
		String GetFileName() const;
		String GetFileExtension() const;

		typedef std::function<Void(UInt32 total, UInt32 ready)> DownloadCallback;

		Buffer Download() const;
		Buffer Download(UInt32 chunks, DownloadCallback callback) const;

	protected:
		static Net::Http CreateSession(
			const TChar* server,
			const UInt16 port,
			const UInt32 options,
			const TChar* userAgent);

		static const String& ConstructRequest(
			const TChar* appVersion,
			const TChar* req);

		Void MoveFrom(Update& other);
		Void CopyFrom(const Update& other);

	private:
		String m_server;
		UInt16 m_port;
		UInt32 m_flags;
		UInt32 m_options;
		String m_userAgent;
		String m_appVersion;
		String m_response;
		ResponseParser m_parser;
	};

	inline Bool Update::IsAvailable() const
	{
		auto remoteVersion = GetLatestVersion();
		auto localVersion = Version(m_appVersion);
		return localVersion < remoteVersion;
	}

	inline Version Update::GetLatestVersion() const
	{
		return Version(m_parser(m_response, UpdateResponseElement::AppVersion));
	}

	inline String Update::GetFileName() const
	{
		auto path = m_parser(m_response, UpdateResponseElement::DownloadPath);
		auto from = path.FindLast(L"/") + 1;
		auto to = path.FindLast(L".");
		auto count = to - from;
		auto file = path.SubString(from, count);
		return file;
	}

	inline String Update::GetFileExtension() const
	{
		auto path = m_parser(m_response, UpdateResponseElement::DownloadPath);
		auto from = path.FindLast(L".");
		auto ext = path.SubString(from + 1);
		return ext;
	}
}