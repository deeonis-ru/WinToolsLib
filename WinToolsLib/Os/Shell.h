#pragma once
#include "..\Types.h"
#include "..\String.h"
#include "..\Exception.h"
#include "..\Handles\Handle.h"

namespace WinToolsLib { namespace Os
{
	enum class KnownFolder
	{
		Unknown = 0,
		CommonPrograms,
		CommonStartMenu,
		CommonStartup,
		CommonTemplates,
		ComputerFolder,
		ConnectionsFolder,
		ControlPanelFolder,
		Cookies,
		Desktop,
		Documents,
		DocumentsLibrary,
		Downloads,
		Favorites,
		Fonts,
		History,
		InternetCache,
		InternetFolder,
		Libraries,
		Links,
		LocalAppData,
		LocalAppDataLow,
		NetworkFolder,
		Profile,
		ProgramData,
		ProgramFiles,
		ProgramFilesX86,
		ProgramFilesCommon,
		ProgramFilesCommonX86,
		Programs,
		Public,
		PublicDesktop,
		PublicDocuments,
		PublicDownloads,
		QuickLaunch,
		Recent,
		RecycleBinFolder,
		RoamingAppData,
		SendTo,
		StartMenu,
		Startup,
		System,
		SystemX86,
		Windows,
		_End
	};

	class Shell
	{
	public:
		static const TChar* KnownFolderToString(KnownFolder folder);
		static KnownFolder KnownFolderFromString(const TChar* string);

		static String GetKnownFolder(KnownFolder folder, const Handles::Handle& token = Handles::Handle());
	};

	class KnownFolderNotSupported : public Exception
	{
	public:
		KnownFolderNotSupported(PCChar function, UInt32 line, PCChar file, KnownFolder folder) :
			Exception(function, line, file, CreateErrorMessage(folder))
		{
		}

		KnownFolderNotSupported(PCChar function, UInt32 line, PCChar file, const TChar* string) :
			Exception(function, line, file, CreateErrorMessage(string))
		{
		}

	protected:
		static String CreateErrorMessage(KnownFolder folder)
		{
			return String::FormatS(Text("Specified known folder %u is not supported"), folder);
		}

		static String CreateErrorMessage(const TChar* string)
		{
			return String::FormatS(Text("Specified known folder %s is not supported"), string);
		}
	};

} }