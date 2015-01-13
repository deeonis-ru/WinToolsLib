#include "Shell.h"
#include "..\System.h"
#include "..\Com\TaskMem.h"

#include <string.h>
#include <tchar.h>

#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")

#include <algorithm>

#define THROW_KNOWN_FOLDER_NOT_SUPPORTED(param) \
	throw KnownFolderNotSupported(__FUNCTION__, __LINE__, __FILE__, param)

namespace WinToolsLib { namespace Os
{
	using namespace Handles;

	struct KnownFolderEntry
	{
		KnownFolder      index;
		REFKNOWNFOLDERID guid;
		UInt32           id;
		const TChar*     string;
	};

	static const KnownFolderEntry g_knownFolder[] =
	{
		{
			KnownFolder::Unknown,
			GUID_NULL,
			0,
			Text("Unknown")
		},
		{
			KnownFolder::CommonPrograms, // COMMON
			FOLDERID_CommonPrograms,     // %ALLUSERSPROFILE%\Microsoft\Windows\Start Menu\Programs
			CSIDL_COMMON_PROGRAMS,       // %ALLUSERSPROFILE%\Start Menu\Programs
			Text("CommonPrograms")
		},
		{
			KnownFolder::CommonStartMenu, // COMMON
			FOLDERID_CommonStartMenu,     // %ALLUSERSPROFILE%\Microsoft\Windows\Start Menu
			CSIDL_COMMON_STARTMENU,       // %ALLUSERSPROFILE%\Start Menu
			Text("CommonStartMenu")
		},
		{
			KnownFolder::CommonStartup, // COMMON
			FOLDERID_CommonStartup,     // %ALLUSERSPROFILE%\Microsoft\Windows\Start Menu\Programs\StartUp
			CSIDL_COMMON_STARTUP,       // %ALLUSERSPROFILE%\Start Menu\Programs\StartUp
			Text("CommonStartup")
		},
		{
			KnownFolder::CommonTemplates, // COMMON
			FOLDERID_CommonTemplates,     // %ALLUSERSPROFILE%\Microsoft\Windows\Templates
			CSIDL_COMMON_TEMPLATES,       // %ALLUSERSPROFILE%\Templates
			Text("CommonTemplates")
		},
		{
			KnownFolder::ComputerFolder,
			FOLDERID_ComputerFolder,
			CSIDL_DRIVES,
			Text("ComputerFolder")
		},
		{
			KnownFolder::ConnectionsFolder,
			FOLDERID_ConnectionsFolder,
			CSIDL_CONNECTIONS,
			Text("ConnectionsFolder")
		},
		{
			KnownFolder::ControlPanelFolder,
			FOLDERID_ControlPanelFolder,
			CSIDL_CONTROLS,
			Text("ControlPanelFolder")
		},
		{
			KnownFolder::Cookies, // PERUSER
			FOLDERID_Cookies,     // %APPDATA%\Microsoft\Windows\Cookies
			CSIDL_COOKIES,        // %USERPROFILE%\Cookies
			Text("Cookies")
		},
		{
			KnownFolder::Desktop, // PERUSER
			FOLDERID_Desktop,     // %USERPROFILE%\Desktop
			CSIDL_DESKTOP,        // %USERPROFILE%\Desktop
			Text("Desktop")
		},
		{
			KnownFolder::Documents, // PERUSER
			FOLDERID_Documents,     // %USERPROFILE%\Documents
			CSIDL_MYDOCUMENTS,      // %USERPROFILE%\My Documents
			Text("Documents")
		},
		{
			KnownFolder::DocumentsLibrary, // PERUSER
			FOLDERID_DocumentsLibrary,     // %APPDATA%\Microsoft\Windows\Libraries\Documents.library-ms
			0,                             // None, value introduced in Windows 7
			Text("DocumentsLibrary")
		},
		{
			KnownFolder::Downloads, // PERUSER
			FOLDERID_Downloads,     // %USERPROFILE%\Downloads
			0,                      // Not applicable
			Text("Downloads")
		},
		{
			KnownFolder::Favorites, // PERUSER
			FOLDERID_Favorites,     // %USERPROFILE%\Favorites
			CSIDL_FAVORITES,        // %USERPROFILE%\Favorites
			Text("Favorites")
		},
		{
			KnownFolder::Fonts, // FIXED
			FOLDERID_Fonts,     // %windir%\Fonts
			CSIDL_FONTS,        // %windir%\Fonts
			Text("Fonts")
		},
		{
			KnownFolder::History, // PERUSER
			FOLDERID_History,     // %LOCALAPPDATA%\Microsoft\Windows\History
			CSIDL_HISTORY,        // %USERPROFILE%\Local Settings\History
			Text("History")
		},
		{
			KnownFolder::InternetCache, // PERUSER
			FOLDERID_InternetCache,     // %LOCALAPPDATA%\Microsoft\Windows\Temporary Internet Files
			CSIDL_INTERNET_CACHE,       // %USERPROFILE%\Local Settings\Temporary Internet Files
			Text("InternetCache")
		},
		{
			KnownFolder::InternetFolder,
			FOLDERID_InternetFolder,
			CSIDL_INTERNET,
			Text("InternetFolder")
		},
		{
			KnownFolder::Libraries, // PERUSER
			FOLDERID_Libraries,     // %APPDATA%\Microsoft\Windows\Libraries
			0,
			Text("Libraries")
		},
		{
			KnownFolder::Links, // PERUSER
			FOLDERID_Links,     // %USERPROFILE%\Links
			0,
			Text("Links")
		},
		{
			KnownFolder::LocalAppData, // PERUSER
			FOLDERID_LocalAppData,     // %LOCALAPPDATA% (%USERPROFILE%\AppData\Local)
			CSIDL_LOCAL_APPDATA,       // %USERPROFILE%\Local Settings\Application Data
			Text("LocalAppData")
		},
		{
			KnownFolder::LocalAppDataLow, // PERUSER
			FOLDERID_LocalAppDataLow,     // %USERPROFILE%\AppData\LocalLow
			0,                            // None
			Text("LocalAppDataLow")
		},
		{
			KnownFolder::NetworkFolder,
			FOLDERID_NetworkFolder,
			CSIDL_NETWORK,
			Text("NetworkFolder")
		},
		{
			KnownFolder::Profile, // FIXED
			FOLDERID_Profile,     // %USERPROFILE% (%SystemDrive%\Users\%USERNAME%)
			CSIDL_PROFILE,        // %USERPROFILE% (%SystemDrive%\Documents and Settings\%USERNAME%)
			Text("Profile")
		},
		{
			KnownFolder::ProgramData, // FIXED
			FOLDERID_ProgramData,     // %ALLUSERSPROFILE% (%ProgramData%, %SystemDrive%\ProgramData)
			CSIDL_COMMON_APPDATA,     // %ALLUSERSPROFILE%\Application Data
			Text("ProgramData")
		},
		{
			KnownFolder::ProgramFiles, // FIXED
			FOLDERID_ProgramFiles,     // %ProgramFiles% (%SystemDrive%\Program Files)
			CSIDL_PROGRAM_FILES,       // %ProgramFiles% (%SystemDrive%\Program Files)
			Text("ProgramFiles")
		},
		{
			KnownFolder::ProgramFilesX86, // FIXED
			FOLDERID_ProgramFilesX86,     // %ProgramFiles% (%SystemDrive%\Program Files)
			CSIDL_PROGRAM_FILESX86,       // %ProgramFiles% (%SystemDrive%\Program Files)
			Text("ProgramFilesX86")
		},
		{
			KnownFolder::ProgramFilesCommon, // FIXED
			FOLDERID_ProgramFilesCommon,     // %ProgramFiles%\Common Files
			CSIDL_PROGRAM_FILES_COMMON,      // %ProgramFiles%\Common Files
			Text("ProgramFilesCommon")
		},
		{
			KnownFolder::ProgramFilesCommonX86, // FIXED
			FOLDERID_ProgramFilesCommonX86,     // %ProgramFiles%\Common Files
			CSIDL_PROGRAM_FILES_COMMONX86,      // %ProgramFiles%\Common Files
			Text("ProgramFilesCommonX86")
		},
		{
			KnownFolder::Programs, // PERUSER
			FOLDERID_Programs,     // %APPDATA%\Microsoft\Windows\Start Menu\Programs
			CSIDL_PROGRAMS,        // %USERPROFILE%\Start Menu\Programs
			Text("Programs")
		},
		{
			KnownFolder::Public, // FIXED
			FOLDERID_Public,     // %PUBLIC% (%SystemDrive%\Users\Public)
			0,                   // None, new for Windows Vista
			Text("Public")
		},
		{
			KnownFolder::PublicDesktop,    // COMMON
			FOLDERID_PublicDesktop,        // %PUBLIC%\Desktop
			CSIDL_COMMON_DESKTOPDIRECTORY, // %ALLUSERSPROFILE%\Desktop
			Text("PublicDesktop")
		},
		{
			KnownFolder::PublicDocuments, // COMMON
			FOLDERID_PublicDocuments,     // %PUBLIC%\Documents
			CSIDL_COMMON_DOCUMENTS,       // %ALLUSERSPROFILE%\Documents
			Text("PublicDocuments")
		},
		{
			KnownFolder::PublicDownloads, // COMMON
			FOLDERID_PublicDownloads,     // %PUBLIC%\Downloads
			0,                            // None, value introduced in Windows Vista
			Text("PublicDownloads")
		},
		{
			KnownFolder::QuickLaunch, // PERUSER
			FOLDERID_QuickLaunch,     // %APPDATA%\Microsoft\Internet Explorer\Quick Launch
			0,                        // None
			Text("QuickLaunch")
		},
		{
			KnownFolder::Recent, // PERUSER
			FOLDERID_Recent,     // %APPDATA%\Microsoft\Windows\Recent
			CSIDL_RECENT,        // %USERPROFILE%\Recent
			Text("Recent")
		},
		{
			KnownFolder::RecycleBinFolder,
			FOLDERID_RecycleBinFolder,
			CSIDL_BITBUCKET,
			Text("RecycleBinFolder")
		},
		{
			KnownFolder::RoamingAppData, // PERUSER
			FOLDERID_RoamingAppData,     // %APPDATA% (%USERPROFILE%\AppData\Roaming)
			CSIDL_APPDATA,               // %APPDATA% (%USERPROFILE%\Application Data)
			Text("RoamingAppData")
		},
		{
			KnownFolder::SendTo, // PERUSER
			FOLDERID_SendTo,     // %APPDATA%\Microsoft\Windows\SendTo
			CSIDL_SENDTO,        // %USERPROFILE%\SendTo
			Text("SendTo")
		},
		{
			KnownFolder::StartMenu, // PERUSER
			FOLDERID_StartMenu,     // %APPDATA%\Microsoft\Windows\Start Menu
			CSIDL_STARTMENU,        // %USERPROFILE%\Start Menu
			Text("StartMenu")
		},
		{
			KnownFolder::Startup, // PERUSER
			FOLDERID_Startup,     // %APPDATA%\Microsoft\Windows\Start Menu\Programs\StartUp
			CSIDL_STARTUP,        // %USERPROFILE%\Start Menu\Programs\StartUp
			Text("Startup")
		},
		{
			KnownFolder::System, // FIXED
			FOLDERID_System,     // %windir%\system32
			CSIDL_SYSTEM,        // %windir%\system32
			Text("System")
		},
		{
			KnownFolder::SystemX86, // FIXED
			FOLDERID_SystemX86,     // %windir%\system32
			CSIDL_SYSTEMX86,        // %windir%\system32
			Text("SystemX86")
		},
		{
			KnownFolder::Windows, // FIXED
			FOLDERID_Windows,     // %windir%
			CSIDL_WINDOWS,        // %windir%
			Text("Windows")
		}
	};

	Bool KnownFolderRegistryIsConsistent()
	{
		auto count = (UInt32)_countof(g_knownFolder);
		auto end = (UInt32)KnownFolder::_End;
		if (count != end)
		{
			return False;
		}
		for (auto i = 0u; i < (UInt32)KnownFolder::_End; i++)
		{
			if (i != (UInt32)g_knownFolder[i].index)
			{
				return False;
			}
		}
		return True;
	}

	static UInt32 ToIndex(KnownFolder folder)
	{
		auto index = (UInt32)folder;
		if (index > (UInt32)KnownFolder::Unknown &&
			index < (UInt32)KnownFolder::_End)
		{
			return index;
		}
		THROW_KNOWN_FOLDER_NOT_SUPPORTED(folder);
	}

	static String ShellGetFolder(UInt32 folder, const Handle& token)
	{
		TChar szPath[MAX_PATH];

		auto result = ::SHGetFolderPath(
			NULL,
			folder,
			token,
			0,
			szPath);

		if (FAILED(result))
		{
			THROW_WIN32_EXCEPTION(result);
		}

		return String(szPath);
	}

	static String ShellGetKnownFolder(REFKNOWNFOLDERID rfid, const Handle& token)
	{
		typedef HRESULT (STDAPICALLTYPE *SHGetKnownFolderPathFunc)(
			__in REFKNOWNFOLDERID rfid,
			__in DWORD /* KNOWN_FOLDER_FLAG */ dwFlags,
			__in_opt HANDLE token,
			__deref_out PWSTR *ppszPath); // free *ppszPath with CoTaskMemFree

		static SHGetKnownFolderPathFunc shGetKnownFolderPath =
			(SHGetKnownFolderPathFunc)[]()
			{
				auto lib = ::LoadLibraryW(L"Shell32.dll");
				return ::GetProcAddress(lib, "SHGetKnownFolderPath");
			}();

		Com::TaskMem<WChar> path;

		auto result = shGetKnownFolderPath(
			rfid,
			0,
			token,
			&path);

		if (FAILED(result))
		{
			THROW_WIN32_EXCEPTION(result);
		}

		return String(path);
	}

	static UInt32 KnownFolderToId(KnownFolder folder)
	{
		auto index = ToIndex(folder);
		auto id = g_knownFolder[index].id;
		if (0 != id)
		{
			return id;
		}
		THROW_KNOWN_FOLDER_NOT_SUPPORTED(folder);
	}

	static REFKNOWNFOLDERID KnownFolderToGuid(KnownFolder folder)
	{
		auto index = ToIndex(folder);
		const auto& guid = g_knownFolder[index].guid;
		if (&guid != &GUID_NULL)
		{
			return guid;
		}
		THROW_KNOWN_FOLDER_NOT_SUPPORTED(folder);
	}

	const TChar* Shell::KnownFolderToString(KnownFolder folder)
	{
		auto index = ToIndex(folder);
		return g_knownFolder[index].string;
	}

	KnownFolder Shell::KnownFolderFromString(const TChar* string)
	{
		auto predicate = [string](const KnownFolderEntry& entry)
		{
			return 0 == _tcsicmp(string, entry.string);
		};
		auto iter = std::find_if(std::begin(g_knownFolder), std::end(g_knownFolder), predicate);
		if (iter != std::end(g_knownFolder))
		{
			return iter->index;
		}
		THROW_KNOWN_FOLDER_NOT_SUPPORTED(string);
	}

	String Shell::GetKnownFolder(KnownFolder folder, const Handle& token)
	{
		if (System::IsVistaOrLater())
		{
			return ShellGetKnownFolder(KnownFolderToGuid(folder), token);
		}
		else
		{
			return ShellGetFolder(KnownFolderToId(folder), token);
		}
	}

} }