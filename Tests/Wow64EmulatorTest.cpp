#include "stdafx.h"
#include <WinToolsLib\Wow64\Emulator.h>
#include <WinToolsLib\System.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(Wow64EmulatorTest)
	{
	public:
		TEST_METHOD(EmulateFsRedirection)
		{
			auto check = [](const TChar* input, const TChar* expectedOutput)
			{
				auto& emulator = Wow64::Emulator::GetInstance();
				Assert::AreEqual(expectedOutput, emulator.EmulateFsRedirection(input));
			};

			check(
				Text("C:\\Windows\\regedit.exe"),
				Text("c:\\windows\\syswow64\\regedit.exe"));

			check(
				Text("C:\\Windows\\System32\\atl100.dll"),
				Text("c:\\windows\\syswow64\\atl100.dll"));

			check(
				Text("C:\\Windows\\System32\\drivers\\wimmount.sys"),
				Text("c:\\windows\\syswow64\\drivers\\wimmount.sys"));

			//
			// Certain subdirectories are exempt from redirection. 
			// Access to these subdirectories is not redirected to %windir%\SysWOW64:
			//
			// %windir%\system32\catroot
			// %windir%\system32\catroot2
			// %windir%\system32\driverstore
			// %windir%\system32\drivers\etc
			// %windir%\system32\logfiles
			// %windir%\system32\spool
			//
			// Windows Server 2008, Windows Vista, Windows Server 2003, and Windows XP:
			// %windir%\system32\driverstore is redirected.
			//

			check(
				Text("C:\\Windows\\System32\\catroot\\{00000000-0000-0000-0000-000000000000}\\ContentCatalog.cat"),
				Text("c:\\windows\\system32\\catroot\\{00000000-0000-0000-0000-000000000000}\\contentcatalog.cat"));

			check(
				Text("C:\\Windows\\System32\\catroot2\\{00000000-0000-0000-0000-000000000000}\\catdb"),
				Text("c:\\windows\\system32\\catroot2\\{00000000-0000-0000-0000-000000000000}\\catdb"));

			if (System::IsSevenOrLater())
			{
				check(
					Text("C:\\Windows\\System32\\DriverStore\\en-US\\volume.inf_loc"),
					Text("c:\\windows\\system32\\driverstore\\en-us\\volume.inf_loc"));
			}
			else
			{
				check(
					Text("C:\\Windows\\System32\\DriverStore\\en-US\\volume.inf_loc"),
					Text("c:\\windows\\syswow64\\driverstore\\en-us\\volume.inf_loc"));
			}

			check(
				Text("C:\\Windows\\System32\\drivers\\etc\\hosts"),
				Text("c:\\windows\\system32\\drivers\\etc\\hosts"));

			check(
				Text("C:\\Windows\\System32\\LogFiles\\Firewall\\pfirewall.log"),
				Text("c:\\windows\\system32\\logfiles\\firewall\\pfirewall.log"));

			check(
				Text("C:\\Windows\\System32\\spool\\drivers\\color\\wsRGB.cdmp"),
				Text("c:\\windows\\system32\\spool\\drivers\\color\\wsrgb.cdmp"));

			check(
				Text("C:\\Windows\\Fonts\\SegoeMarker.ttf"),
				Text("c:\\windows\\fonts\\segoemarker.ttf"));
		}
	};
}