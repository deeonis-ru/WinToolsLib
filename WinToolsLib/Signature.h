#pragma once
#include <Windows.h>
#include <WinCrypt.h>
#include <WinTrust.h>
#include <mscat.h>

#include "Types.h"
#include "Buffer.h"
#include "Handles\Handle.h"

namespace WinToolsLib
{
	using namespace Handles;

	class Signature
	{
	public:
		enum class State
		{
			Unknown,
			IsTrusted,
			SubjectNotTrusted,
			UntrustedRoot,
			ProviderUnknown,
			ActionUnknown,
			SubjectFromUnknown,
			CertificateRevoked,
			CertificateExpired,
			BadDigest,
			NoSignature,
			FileNotFound
		};

		Signature(const String& filePath);

		Bool IsValid();
		State Check();

		StringW GetSigner();
		CATALOG_INFO& GetCatalogInfo();

	private:
		String m_filePath;
		CATALOG_INFO m_catalogInfo;

		State WinErrorToState(const Int32 err);
		Int32 IsTrusted(WINTRUST_DATA* pWintrustData);
		Int32 IsTrusted(const Buffer& fileHash, const HCATADMIN& adminContext);
		Int32 IsTrusted();
		Buffer CalcFileHash(const Handle& fileHandle, const HCATADMIN& adminContext);
		Buffer CalcFileHash(const TChar* filePath, const HCATADMIN& adminContext);
	};

	inline CATALOG_INFO& Signature::GetCatalogInfo()
	{
		return m_catalogInfo;
	}
}