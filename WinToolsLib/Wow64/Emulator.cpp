#include "Emulator.h"
#include "..\FileSystem\Path.h"
#include "..\Os\Shell.h"

using namespace WinToolsLib::FileSystem;
using namespace WinToolsLib::Os;

namespace WinToolsLib
{
	namespace Wow64
	{
		Emulator& Emulator::GetInstance()
		{
			static Emulator instance;
			return instance;
		}

		String Emulator::EmulateFsRedirection(const TChar* path)
		{
			const auto getKnownFolder = [](KnownFolder folder, const TChar* subfolder)
			{
				Path path = Os::Shell::GetKnownFolder(folder);
				if (nullptr != subfolder)
				{
					path.Append(subfolder);
				}
				auto string = path.ToString();
				string.ToLower();
				return string;
			};

			static const auto windir = getKnownFolder(KnownFolder::Windows, nullptr);
			static const auto system32 = getKnownFolder(KnownFolder::System, nullptr);
			static const auto sysWow64 = getKnownFolder(KnownFolder::SystemX86, nullptr);

			static const auto catroot = getKnownFolder(KnownFolder::System, Text("catroot\\*"));
			static const auto catroot2 = getKnownFolder(KnownFolder::System, Text("catroot2\\*"));
			static const auto driverstore = getKnownFolder(KnownFolder::System, Text("driverstore\\*"));
			static const auto driversetc = getKnownFolder(KnownFolder::System, Text("drivers\\etc\\*"));
			static const auto logfiles = getKnownFolder(KnownFolder::System, Text("logfiles\\*"));
			static const auto spool = getKnownFolder(KnownFolder::System, Text("spool\\*"));

			const auto isExclusion = [&](const String& filePath)
			{
				if (filePath.Match(catroot) ||
					filePath.Match(catroot2) ||
					filePath.Match(driverstore) ||
					filePath.Match(driversetc) ||
					filePath.Match(logfiles) ||
					filePath.Match(spool))
				{
					return True;
				}

				return False;
			};

			String filePath(path);
			filePath.ToLower();
			if (!isExclusion(filePath))
			{
				if (filePath.BeginsWith(system32))
				{
					filePath.Replace(system32, sysWow64);
				}
				else
				{
					Path fullPath(filePath);
					if (windir == fullPath.GetFolder())
					{
						filePath.Replace(windir, sysWow64);
					}
				}
			}
			return filePath;
		}
	}
}
