#include "Signature.h"
#include "Log.h"
#include "Exception.h"
#include "Process.h"
#include "DynamicFunction.h"
#include "FileSystem\File.h"

#include <SoftPub.h>
#include <WinTrust.h>
#pragma comment(lib, "Wintrust.lib")
#include <WinCrypt.h>
#pragma comment(lib, "Crypt32.lib")

#if (NTDDI_VERSION <= NTDDI_WIN7)
typedef struct WINTRUST_CATALOG_INFO_8_
{
    DWORD           cbStruct;               // = sizeof(WINTRUST_CATALOG_INFO)

    DWORD           dwCatalogVersion;       // optional: Catalog version number
    LPCWSTR         pcwszCatalogFilePath;   // required: path/name to Catalog file

    LPCWSTR         pcwszMemberTag;         // optional: tag to member in Catalog
    LPCWSTR         pcwszMemberFilePath;    // required: path/name to member file
    HANDLE          hMemberFile;            // optional: open handle to pcwszMemberFilePath

    _Field_size_(cbCalculatedFileHash) BYTE            *pbCalculatedFileHash;  // optional: pass in the calculated hash
    DWORD           cbCalculatedFileHash;   // optional: pass in the count bytes of the calc hash

    PCCTL_CONTEXT   pcCatalogContext;       // optional: pass in to use instead of CatalogFilePath.

    HCATADMIN       hCatAdmin;              // optional for SHA-1 hashes, required for all other hash types.

} WINTRUST_CATALOG_INFO_8, *PWINTRUST_CATALOG_INFO_8;


typedef struct _CERT_STRONG_SIGN_SERIALIZED_INFO {
    DWORD                   dwFlags;
    LPWSTR                  pwszCNGSignHashAlgids;
    LPWSTR                  pwszCNGPubKeyMinBitLengths; // Optional
} CERT_STRONG_SIGN_SERIALIZED_INFO, *PCERT_STRONG_SIGN_SERIALIZED_INFO;

typedef struct _CERT_STRONG_SIGN_PARA {
    DWORD                   cbSize;

    DWORD                   dwInfoChoice;
    union  {
        void                                *pvInfo;

        // CERT_STRONG_SIGN_SERIALIZED_INFO_CHOICE
        PCERT_STRONG_SIGN_SERIALIZED_INFO   pSerializedInfo;

        // CERT_STRONG_SIGN_OID_INFO_CHOICE
        LPSTR                               pszOID;
        
    } DUMMYUNIONNAME;
} CERT_STRONG_SIGN_PARA, *PCERT_STRONG_SIGN_PARA;

typedef const CERT_STRONG_SIGN_PARA *PCCERT_STRONG_SIGN_PARA;
#endif

namespace WinToolsLib
{
	class CertStore
	{
	public:
		CertStore(HCERTSTORE hStore = NULL);
		~CertStore();

		HCERTSTORE* GetAddress();
		operator HCERTSTORE() const;

		HCERTSTORE reset();
		CertStore& operator=(const HCERTSTORE hStore);

	private:
		HCERTSTORE m_hStore;
	};

	CertStore::CertStore(HCERTSTORE hStore) :
		m_hStore(hStore)
	{
	}

	CertStore::~CertStore()
	{
		if (NULL != m_hStore)
		{
			::CertCloseStore(m_hStore, 0);
		}
	}

	HCERTSTORE* CertStore::GetAddress()
	{
		return &m_hStore;
	}

	CertStore::operator HCERTSTORE() const
	{
		return m_hStore;
	}

	HCERTSTORE CertStore::reset()
	{
		auto tmp = m_hStore;
		m_hStore = NULL;
		return tmp;
	}

	CertStore& CertStore::operator=(const HCERTSTORE hStore)
	{
		if (NULL != m_hStore)
		{
			::CertCloseStore(m_hStore, 0);
		}
		m_hStore = hStore;
		return *this;
	}

	//
	// HCRYPTMSG wrapper
	//

	class CryptMsg
	{
	public:
		CryptMsg(HCRYPTMSG hMsg = NULL);
		~CryptMsg();

		HCRYPTMSG* GetAddress();
		operator HCRYPTMSG() const;

		HCRYPTMSG reset();
		CryptMsg& operator=(const HCRYPTMSG hMsg);

	private:
		HCRYPTMSG m_hMsg;
	};

	CryptMsg::CryptMsg(HCRYPTMSG hMsg) :
		m_hMsg(hMsg)
	{
	}

	CryptMsg::~CryptMsg()
	{
		if (NULL != m_hMsg)
		{
			::CryptMsgClose(m_hMsg);
		}
	}

	HCRYPTMSG* CryptMsg::GetAddress()
	{
		return &m_hMsg;
	}

	CryptMsg::operator HCRYPTMSG() const
	{
		return m_hMsg;
	}

	HCERTSTORE CryptMsg::reset()
	{
		auto tmp = m_hMsg;
		m_hMsg = NULL;
		return tmp;
	}

	CryptMsg& CryptMsg::operator=(const HCRYPTMSG hMsg)
	{
		if (NULL != m_hMsg)
		{
			::CryptMsgClose(m_hMsg);
		}
		m_hMsg = hMsg;
		return *this;
	}

	//
	// PCCERT_CONTEXT wrapper
	//

	class CertificateContext
	{
	public:
		CertificateContext(PCCERT_CONTEXT context);
		~CertificateContext();

		operator PCCERT_CONTEXT() const;

	private:
		PCCERT_CONTEXT m_context;
	};

	CertificateContext::CertificateContext(PCCERT_CONTEXT context) :
		m_context(context)
	{
	}

	CertificateContext::~CertificateContext()
	{
		::CertFreeCertificateContext(m_context);
	}

	CertificateContext::operator PCCERT_CONTEXT() const
	{
		return m_context;
	}

	//
	// HCATADMIN wrapper
	//

	class CryptCatAdminContext
	{
		CryptCatAdminContext(const CryptCatAdminContext& other) {};
	public:
		CryptCatAdminContext();
		~CryptCatAdminContext();

		operator HCATADMIN() const;

	private:
		HCATADMIN m_hCatAdmin;
	};

	CryptCatAdminContext::CryptCatAdminContext() :
		m_hCatAdmin(NULL)
	{
		BOOL success = FALSE;

		typedef BOOL (WINAPI *Fn)(HCATADMIN *, const GUID *, PCWSTR, PCCERT_STRONG_SIGN_PARA, DWORD);
		static const auto fn = DynamicFunction<Fn>("Wintrust", "CryptCATAdminAcquireContext2");

		if (fn)
		{
			success = fn(&m_hCatAdmin, NULL, BCRYPT_SHA256_ALGORITHM, NULL, 0);
		}
		else
		{
			success = ::CryptCATAdminAcquireContext(&m_hCatAdmin, NULL, 0);
		}

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	CryptCatAdminContext::~CryptCatAdminContext()
	{
		BOOL success = ::CryptCATAdminReleaseContext(m_hCatAdmin, 0);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	//
	// HCATINFO wrapper
	//

	CryptCatAdminContext::operator HCATADMIN() const
	{
		return m_hCatAdmin;
	}

	class CryptCatAdminCatalogContext
	{
	public:
		CryptCatAdminCatalogContext(const Buffer& fileHash, const HCATADMIN adminContext);
		~CryptCatAdminCatalogContext();

		operator HCATINFO() const;
		Bool NextHCatInfo();
		Void GetInfo(CATALOG_INFO* pCatalogInfo) const;

	private:
		Buffer m_fileHash;
		HCATINFO m_hCatInfo;
		HCATADMIN m_context;
	};

	CryptCatAdminCatalogContext::CryptCatAdminCatalogContext(const Buffer& fileHash, const HCATADMIN adminContext) :
		m_hCatInfo(NULL),
		m_fileHash(fileHash),
		m_context(adminContext)
	{
		m_hCatInfo = ::CryptCATAdminEnumCatalogFromHash(m_context, (BYTE*)m_fileHash.GetBuffer(), (DWORD)m_fileHash.GetSize(), 0, NULL);
	}

	CryptCatAdminCatalogContext::~CryptCatAdminCatalogContext()
	{
		if (m_hCatInfo)
			::CryptCATAdminReleaseCatalogContext(m_context, m_hCatInfo, 0);
	}

	CryptCatAdminCatalogContext::operator HCATINFO() const
	{
		return m_hCatInfo;
	}

	Void CryptCatAdminCatalogContext::GetInfo(CATALOG_INFO* pCatalogInfo) const
	{
		BOOL success = ::CryptCATCatalogInfoFromContext(m_hCatInfo, pCatalogInfo, 0);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Bool CryptCatAdminCatalogContext::NextHCatInfo()
	{
		Bool res = False;

		m_hCatInfo = ::CryptCATAdminEnumCatalogFromHash(m_context, (BYTE*)m_fileHash.GetBuffer(), (DWORD)m_fileHash.GetSize(), 0, &m_hCatInfo);
		if (NULL != m_hCatInfo)
			res = True;

		return res;
	}

	//
	// Signature
	//

	Signature::Signature(const String& filePath)
		: m_filePath(filePath)
	{
		m_catalogInfo.cbStruct = 0;
		m_catalogInfo.wszCatalogFile[0] = 0;
	}

	Bool Signature::IsValid()
	{
		auto state = Check();
		return state == State::IsTrusted;
	}

	Signature::State Signature::Check()
	{
		try
		{
			CryptCatAdminContext adminContext;

			Buffer fileHash = CalcFileHash(m_filePath, adminContext);

			State signatureState = State::Unknown;
			CryptCatAdminCatalogContext catalogContext(fileHash, adminContext);
			while (catalogContext != NULL)
			{
				m_catalogInfo.cbStruct = sizeof(CATALOG_INFO);
				catalogContext.GetInfo(&m_catalogInfo);
				auto res = IsTrusted(fileHash, adminContext);

				signatureState = WinErrorToState(res);
				if (State::IsTrusted == signatureState)
					return signatureState;

				catalogContext.NextHCatInfo();
			}

			if (sizeof(CATALOG_INFO) == m_catalogInfo.cbStruct)
			{
				return signatureState;
			}
			else
			{
				auto res = IsTrusted();
				return WinErrorToState(res);
			}
		}
		catch (FileNotFoundException&)
		{
			return State::FileNotFound;
		}
		catch (...)
		{
			LOG_WARNING(Text("Signature: failed to check %s"), m_filePath.GetBuffer());
			return State::Unknown;
		}
	}

	StringW Signature::GetSigner()
	{
		DWORD dwEncoding, dwContentType, dwFormatType;
		CertStore certStore;
		CryptMsg cryptMsg;

		// Get message handle and store handle from the signed file.
		auto success = ::CryptQueryObject(
			CERT_QUERY_OBJECT_FILE,
			m_filePath,
			CERT_QUERY_CONTENT_FLAG_ALL,
			CERT_QUERY_FORMAT_FLAG_BINARY,
			0,
			&dwEncoding,
			&dwContentType,
			&dwFormatType,
			certStore.GetAddress(),
			cryptMsg.GetAddress(),
			NULL);

		// Special for .cat files
		if (!success && /*CERT_QUERY_CONTENT_CTL == dwContentType &&*/ m_catalogInfo.cbStruct != 0)
		{
			CertStore tmpCertStore;
			CryptMsg tmpCryptMsg;

			success = ::CryptQueryObject(
				CERT_QUERY_OBJECT_FILE,
				m_catalogInfo.wszCatalogFile,
				CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
				CERT_QUERY_FORMAT_FLAG_BINARY,
				0,
				&dwEncoding,
				&dwContentType,
				&dwFormatType,
				tmpCertStore.GetAddress(),
				tmpCryptMsg.GetAddress(),
				NULL);

			certStore = tmpCertStore.reset();
			cryptMsg = tmpCryptMsg.reset();
		}

		if (!success)
		{
			return StringW();
		}

		DWORD dwSignerInfo = 0;

		// Get signer information size.
		success = ::CryptMsgGetParam(
			cryptMsg,
			CMSG_SIGNER_INFO_PARAM,
			0,
			NULL,
			&dwSignerInfo);

		if (!success)
		{
			return StringW();
		}

		// Allocate memory for signer information.
		Buffer signerInfo(dwSignerInfo);
		auto pSignerInfo = (PCMSG_SIGNER_INFO)signerInfo.GetBuffer();

		// Get signer information.
		success = ::CryptMsgGetParam(
			cryptMsg,
			CMSG_SIGNER_INFO_PARAM,
			0,
			pSignerInfo,
			&dwSignerInfo);

		if (!success)
		{
			return StringW();
		}

		CERT_INFO certInfo;
		certInfo.Issuer = pSignerInfo->Issuer;
		certInfo.SerialNumber = pSignerInfo->SerialNumber;

		// Search for the signer certificate in the temporary certificate store.
		CertificateContext certContext = ::CertFindCertificateInStore(
			certStore,
			X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
			0,
			CERT_FIND_SUBJECT_CERT,
			(PVOID)&certInfo,
			NULL);

		if (!certContext)
		{
			return StringW();
		}

		// Get subject name size.
		DWORD dwSubjectNameSize = ::CertGetNameString(
			certContext, 
			CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			NULL,
			0);

		StringW strSubjectName;

		// Allocate memory for subject name.
		strSubjectName.Resize(dwSubjectNameSize - 1);

		// Get subject name.
		::CertGetNameString(
			certContext, 
			CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			strSubjectName.GetBuffer(),
			dwSubjectNameSize);

		return strSubjectName;
	}

	Signature::State Signature::WinErrorToState(const Int32 err)
	{
		State res = State::Unknown;

		switch (err)
		{
		case ERROR_SUCCESS:
			res = State::IsTrusted;
			break;
		case TRUST_E_SUBJECT_NOT_TRUSTED:
			res = State::SubjectNotTrusted;
			break;
		case TRUST_E_PROVIDER_UNKNOWN:
			res = State::ProviderUnknown;
			break;
		case TRUST_E_ACTION_UNKNOWN:
			res = State::ActionUnknown;
			break;
		case TRUST_E_SUBJECT_FORM_UNKNOWN:
			res = State::SubjectFromUnknown;
			break;
		case CERT_E_REVOKED:
			res = State::CertificateRevoked;
			break;
		case CERT_E_EXPIRED:
			res = State::CertificateExpired;
			break;
		case TRUST_E_BAD_DIGEST:
			res = State::BadDigest;
			break;
		case TRUST_E_NOSIGNATURE:
			res = State::NoSignature;
			break;
		case CERT_E_UNTRUSTEDROOT:
			res = State::UntrustedRoot;
			break;
		}

		return res;
	}

	Int32 Signature::IsTrusted(WINTRUST_DATA* pWintrustData)
	{
		GUID actionGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;

		auto result = ::WinVerifyTrust(0, &actionGuid, pWintrustData);
		if (ERROR_SUCCESS == result)
		{
			pWintrustData->dwStateAction = WTD_STATEACTION_CLOSE;
			::WinVerifyTrust(0, &actionGuid, pWintrustData);
		}
		// Based on "Example C Program: Verifying the Signature of a PE File" from MSDN
		else if (TRUST_E_NOSIGNATURE == result)
		{
			result = ::GetLastError();
		}

		return result;
	}

	Int32 Signature::IsTrusted(const Buffer& fileHash, const HCATADMIN& adminContext)
	{		
		String memberTag = Buffer::ToString(fileHash);

		WINTRUST_CATALOG_INFO_8 wintrustCatalogInfo = {0};
		wintrustCatalogInfo.cbStruct = sizeof(WINTRUST_CATALOG_INFO_8);
		wintrustCatalogInfo.dwCatalogVersion = 0;
		wintrustCatalogInfo.pcwszCatalogFilePath = m_catalogInfo.wszCatalogFile;
		wintrustCatalogInfo.pcwszMemberTag = memberTag;
		wintrustCatalogInfo.pcwszMemberFilePath = m_filePath;
		wintrustCatalogInfo.hMemberFile = NULL;
		wintrustCatalogInfo.hCatAdmin = adminContext;

		WINTRUST_DATA wintrustData = {0};
		wintrustData.cbStruct = sizeof(WINTRUST_DATA);
		wintrustData.pPolicyCallbackData = 0;
		wintrustData.pSIPClientData = 0;
		wintrustData.dwUIChoice = WTD_UI_NONE;
		wintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
		wintrustData.dwUnionChoice = WTD_CHOICE_CATALOG;
		wintrustData.pCatalog = (WINTRUST_CATALOG_INFO*)&wintrustCatalogInfo;
		wintrustData.dwStateAction = WTD_STATEACTION_VERIFY;
		wintrustData.hWVTStateData = NULL;
		wintrustData.pwszURLReference = NULL;
		wintrustData.dwProvFlags = 0;
		wintrustData.dwUIContext = WTD_UICONTEXT_EXECUTE;

		return IsTrusted(&wintrustData);
	}

	Int32 Signature::IsTrusted()
	{
		WINTRUST_FILE_INFO wintrustFileInfo = {0};
		wintrustFileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
		wintrustFileInfo.pcwszFilePath = m_filePath;
		wintrustFileInfo.hFile = NULL;
		wintrustFileInfo.pgKnownSubject = NULL;

		WINTRUST_DATA wintrustData = {0};
		wintrustData.cbStruct = sizeof(WINTRUST_DATA);
		wintrustData.dwUnionChoice = WTD_CHOICE_FILE;
		wintrustData.pFile = &wintrustFileInfo;
		wintrustData.dwUIChoice = WTD_UI_NONE;
		wintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
		wintrustData.dwStateAction = WTD_STATEACTION_IGNORE;
		wintrustData.dwProvFlags = WTD_SAFER_FLAG;
		wintrustData.hWVTStateData = NULL;
		wintrustData.pwszURLReference = NULL;

		return IsTrusted(&wintrustData);
	}

	Buffer Signature::CalcFileHash(const Handle& fileHandle, const HCATADMIN& adminContext)
	{
		DWORD hashSize = 0;

		typedef BOOL (WINAPI *Fn)(HCATADMIN, HANDLE, DWORD *, BYTE *, DWORD);
		static const auto fn = DynamicFunction<Fn>("Wintrust", "CryptCATAdminCalcHashFromFileHandle2");
		if (fn)
		{
			fn(adminContext, fileHandle, &hashSize, NULL, 0);
			if (0 == hashSize)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			Buffer fileHash(hashSize);

			BOOL success = fn(adminContext, fileHandle, &hashSize, fileHash.GetBuffer(), 0);
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			return fileHash;
		}
		else
		{
			::CryptCATAdminCalcHashFromFileHandle(fileHandle, &hashSize, NULL, 0);
			if (0 == hashSize)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			Buffer fileHash(hashSize);

			BOOL success = ::CryptCATAdminCalcHashFromFileHandle(fileHandle, &hashSize, fileHash.GetBuffer(), 0);
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			return fileHash;
		}
	}

	Buffer Signature::CalcFileHash(const TChar* filePath, const HCATADMIN& adminContext)
	{
		if (!FileSystem::File::Exists(filePath))
		{
			THROW_FILE_NOT_FOUND_EXCEPTION(filePath);
		}

		Handle fileHandle = ::CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
		if (!fileHandle.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return CalcFileHash(fileHandle, adminContext);
	}
}