#include "stdafx.h"
#include <WinToolsLib\FileSystem\Path.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;
using namespace WinToolsLib::FileSystem;

namespace Tests
{
	TEST_CLASS(PathTest)
	{
	public:
		TEST_METHOD(PathAppend)
		{
			[]() // Simplest case
			{
				Path path;
				Assert::AreEqual(Text(""), static_cast<const String&>(path));

				path.Append(Text("Dir"));
				Assert::AreEqual(Text("Dir"), static_cast<const String&>(path));
			
				path.Append(Text("SubDir"));
				Assert::AreEqual(Text("Dir\\SubDir"), static_cast<const String&>(path));
			
				path.Append(Text(""));
				Assert::AreEqual(Text("Dir\\SubDir"), static_cast<const String&>(path));
			}();

			[]() // More backslashes
			{
				Path path(Text("Dir\\"));
				Assert::AreEqual(Text("Dir\\"), static_cast<const String&>(path));

				path.Append(Text("\\SubDir"));
				Assert::AreEqual(Text("Dir\\SubDir"), static_cast<const String&>(path));
			
				path.Append(Text("\\FileName.Ext"));
				Assert::AreEqual(Text("Dir\\SubDir\\FileName.Ext"), static_cast<const String&>(path));
			}();

			[]() // Several paths
			{
				Path path(Text("RootDir\\"));

				path.Append(Path(Text("Some\\Nested\\Dirs\\")));
				Assert::AreEqual(Text("RootDir\\Some\\Nested\\Dirs\\"), static_cast<const String&>(path));

				path.Append(Path(Text("FileName.Ext")));
				Assert::AreEqual(Text("RootDir\\Some\\Nested\\Dirs\\FileName.Ext"), static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(PathReplaceName)
		{
			[]() // Simplest case
			{
				Path path(Text("C:\\Dir\\SubDir\\FileName.Ext"));

				path.ReplaceName(Text("AnotherName"));
				Assert::AreEqual(Text("AnotherName.Ext"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("C:\\Dir\\SubDir\\AnotherName.Ext"), static_cast<const String&>(path));

				path.ReplaceName(Text("Small"));
				Assert::AreEqual(Text("Small.Ext"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("C:\\Dir\\SubDir\\Small.Ext"), static_cast<const String&>(path));
			}();

			[]() // No extension
			{
				Path path(Text("C:\\Dir\\SubDir\\FileName"));

				path.ReplaceName(Text("AnotherName"));
				Assert::AreEqual(Text("AnotherName"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("C:\\Dir\\SubDir\\AnotherName"), static_cast<const String&>(path));
			}();

			[]() // No backslashes
			{
				Path path(Text("FileName.Ext"));

				path.ReplaceName(Text("AnotherName"));
				Assert::AreEqual(Text("AnotherName.Ext"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("AnotherName.Ext"), static_cast<const String&>(path));

				path.ReplaceName(Text("Small"));
				Assert::AreEqual(Text("Small.Ext"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("Small.Ext"), static_cast<const String&>(path));
			}();

			[]() // No extension and no backslashes
			{
				Path path(Text("FileName"));

				path.ReplaceName(Text("AnotherName"));
				Assert::AreEqual(Text("AnotherName"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("AnotherName"), static_cast<const String&>(path));

				path.ReplaceName(Text("Small"));
				Assert::AreEqual(Text("Small"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("Small"), static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(PathReplaceFullName)
		{
			[]() // Full path
			{
				Path path(Text("C:\\Dir\\SubDir\\Name.Ext"));

				path.ReplaceFullName(Text("FullName.WithExt"));
				Assert::AreEqual(Text("FullName.WithExt"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("C:\\Dir\\SubDir\\FullName.WithExt"), static_cast<const String&>(path));

				path.ReplaceFullName(Text("FullNameWithOutExt"));
				Assert::AreEqual(Text("FullNameWithOutExt"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("C:\\Dir\\SubDir\\FullNameWithOutExt"), static_cast<const String&>(path));
			}();

			[]() // No backslashes
			{
				Path path(Text("Name.Ext"));

				path.ReplaceFullName(Text("FullNameWithOutExt"));
				Assert::AreEqual(Text("FullNameWithOutExt"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("FullNameWithOutExt"), static_cast<const String&>(path));

				path.ReplaceFullName(Text("FullName.WithExt"));
				Assert::AreEqual(Text("FullName.WithExt"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("FullName.WithExt"), static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(PathReplaceExtension)
		{
			[]() // Simplest case
			{
				Path path(Text("C:\\Dir\\SubDir\\FileName.Ext"));

				path.ReplaceExtension(Text("AnotherExt"));
				Assert::AreEqual(Text("FileName.AnotherExt"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("C:\\Dir\\SubDir\\FileName.AnotherExt"), static_cast<const String&>(path));

				path.ReplaceExtension(Text("SomeExt"));
				Assert::AreEqual(Text("FileName.SomeExt"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("C:\\Dir\\SubDir\\FileName.SomeExt"), static_cast<const String&>(path));
			}();

			[]() // Without extension
			{
				Path path(Text("C:\\Dir\\SubDir\\FileName"));

				path.ReplaceExtension(Text("Ext"));
				Assert::AreEqual(Text("FileName.Ext"), static_cast<const String&>(path.GetName()));
				Assert::AreEqual(Text("C:\\Dir\\SubDir\\FileName.Ext"), static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(PathReplaceDirectory)
		{
			[]() // Simplest case
			{
				Path path(Text("D:\\Dir\\SubDir\\FileName.Ext"));

				path.ReplaceDirectory(Text("C:\\AnotherDir\\OneMore"));
				Assert::AreEqual(Text("C:\\AnotherDir\\OneMore\\FileName.Ext"), static_cast<const String&>(path));
			}();

			[]() // Without backslashes
			{
				Path path(Text("FileName"));

				path.ReplaceDirectory(Text("Dir\\SubDir"));
				Assert::AreEqual(Text("Dir\\SubDir\\FileName"), static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(PathGetFileName)
		{
			auto check = [](const TChar* input, const TChar* expectedOutput)
			{
				auto path = Path::GetFileName(input);
				Assert::AreEqual(expectedOutput, static_cast<const String&>(path));
			};

			check(
				Text(R"(C:\Windows\System32\StikyNot.exe)"),
				Text(R"(C:\Windows\System32\StikyNot.exe)"));

			check(
				Text(R"("C:\Program Files\Microsoft Device Center\itype.exe")"),
				Text(R"(C:\Program Files\Microsoft Device Center\itype.exe)"));

			check(
				Text(R"("C:\Program Files\Microsoft Office\Office14\BCSSync.exe" /DelayServices)"),
				Text(R"(C:\Program Files\Microsoft Office\Office14\BCSSync.exe)"));

			check(
				Text(R"(C:\Windows\system32\rundll32.exe C:\Windows\System32\LogiLDA.dll,LogiFetch)"),
				Text(R"(C:\Windows\System32\LogiLDA.dll)"));
			
			check(
				Text(R"(C:\Windows\system32\Rundll32.exe C:\Windows\System32\LogiLDA.dll,LogiFetch)"),
				Text(R"(C:\Windows\System32\LogiLDA.dll)"));
			
			check(
				Text(R"(C:\Windows\system32\RunDll32.exe "D:\Foo Bar\bad.dll",DllRun)"),
				Text(R"(D:\Foo Bar\bad.dll)"));
			
			check(
				Text(R"("C:\Windows\system32\RunDll32.exe" "D:\Foo Bar\bad.dll",DllRun)"),
				Text(R"(D:\Foo Bar\bad.dll)"));

			check(
				Text(R"(C:\Program Files\NVIDIA Corporation\Installer2\installer.{DC9636E6-1588-49A3-9AA2-9BC0C34AA837}\NVI2.dll,0)"),
				Text(R"(C:\Program Files\NVIDIA Corporation\Installer2\installer.{DC9636E6-1588-49A3-9AA2-9BC0C34AA837}\NVI2.dll)"));

			check(
				Text(R"(C:\Program Files\Common Files\Microsoft Shared\OFFICE14\Office Setup Controller\OSETUP.DLL,1)"),
				Text(R"(C:\Program Files\Common Files\Microsoft Shared\OFFICE14\Office Setup Controller\OSETUP.DLL)"));

			check(
				Text(R"(C:\Windows\Microsoft.NET\Framework64\v4.0.30319\SetupCache\\DisplayIcon.ico)"),
				Text(R"(C:\Windows\Microsoft.NET\Framework64\v4.0.30319\SetupCache\DisplayIcon.ico)"));

			check(
				Text(R"(C:\Program Files\Windows Defender\MSASCui.exe -hide)"),
				Text(R"(C:\Program Files\Windows Defender\MSASCui.exe)"));
			
			check(
				Text(R"(C:\Program Files\Windows Defender\MSASCui.EXE -hide)"),
				Text(R"(C:\Program Files\Windows Defender\MSASCui.EXE)"));
			
			check(
				Text(R"(\??\C:\WINDOWS\system32\winlogon.exe)"),
				Text(R"(C:\WINDOWS\system32\winlogon.exe)"));
			
		}
		
		TEST_METHOD(PathNtToWin32)
		{
			// Check kernel convertion rules
			{
				auto check = [](const TChar* input, const TChar* expectedOutput)
				{
					auto path = Path::NtToWin32(input, True);
					Assert::AreEqual(expectedOutput, static_cast<const String&>(path));
				};

				check(
					Text("\\SystemRoot\\System32\\ntoskrnl.exe"),
					Text("C:\\Windows\\System32\\ntoskrnl.exe"));

				check(
					Text("\\SystemRoot\\System32\\Drivers\\Ntfs.sys"),
					Text("C:\\Windows\\System32\\Drivers\\Ntfs.sys"));

				check(
					Text("\\??\\C:\\Windows\\system32\\drivers\\hcmon.sys"),
					Text("C:\\Windows\\system32\\drivers\\hcmon.sys"));

				check(
					Text("\\WINDOWS\\system32\\ntkrnlpa.exe"),
					Text("C:\\WINDOWS\\system32\\ntkrnlpa.exe"));

				check(
					Text("pci.sys"),
					Text("C:\\Windows\\System32\\Drivers\\pci.sys"));
			}

			// Check non kernel convertion rules
			{
				auto check = [](const TChar* input, const TChar* expectedOutput)
				{
					auto path = Path::NtToWin32(input, False);
					Assert::AreEqual(expectedOutput, static_cast<const String&>(path));
				};

				check(
					Text("\\SystemRoot\\System32\\ntoskrnl.exe"),
					Text("C:\\Windows\\System32\\ntoskrnl.exe"));

				check(
					Text("\\SystemRoot\\System32\\Drivers\\Ntfs.sys"),
					Text("C:\\Windows\\System32\\Drivers\\Ntfs.sys"));

				check(
					Text("\\??\\C:\\WINDOWS\\system32\\winlogon.exe"),
					Text("C:\\WINDOWS\\system32\\winlogon.exe"));

				check(
					Text("\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe"),
					Text("\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe"));

				check(
					Text("services.exe"),
					Text("services.exe"));
			}
		}
		
		TEST_METHOD(PathGetName)
		{
			auto check = [](const TChar* input, const TChar* expectedOutput)
			{
				Path path(input);
				Assert::AreEqual(expectedOutput, path.GetName());
			};

			check(
				Text("C:\\Windows\\System32\\StikyNot.exe"),
				Text("StikyNot.exe"));

			check(
				Text("C:\\Windows\\System32\\"),
				Text(""));

			check(
				Text("C:\\Windows"),
				Text("Windows"));

			check(
				Text("services.exe"),
				Text("services.exe"));

			check(
				Text("\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe"),
				Text("App.exe"));

			check(
				Text("\\SystemRoot\\System32\\ntoskrnl.exe"),
				Text("ntoskrnl.exe"));

			check(
				Text("\\SystemRoot"),
				Text("SystemRoot"));
		}
		
		TEST_METHOD(PathGetNameWithoutExtension)
		{
			auto check = [](const TChar* input, const TChar* expectedOutput)
			{
				Path path(input);
				Assert::AreEqual(expectedOutput, path.GetNameWithoutExtension());
			};

			check(
				Text("C:\\Windows\\System32\\StikyNot.exe"),
				Text("StikyNot"));

			check(
				Text("C:\\Some.Folder\\SomeFile"),
				Text("SomeFile"));

			check(
				Text("C:\\SomeFolder\\.SomeFile"),
				Text(""));

			check(
				Text("C:\\Windows\\System32\\"),
				Text(""));

			check(
				Text("C:\\Windows"),
				Text("Windows"));

			check(
				Text("services.exe"),
				Text("services"));

			check(
				Text("\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe"),
				Text("App"));

			check(
				Text("\\SystemRoot\\System32\\ntoskrnl.exe"),
				Text("ntoskrnl"));

			check(
				Text("\\SystemRoot"),
				Text("SystemRoot"));
		}
		
		TEST_METHOD(PathGetFolder)
		{
			auto check = [](const TChar* input, const TChar* expectedOutput)
			{
				Path path(input);
				Assert::AreEqual(expectedOutput, path.GetFolder());
			};

			check(
				Text("C:\\Windows\\System32\\StikyNot.exe"),
				Text("C:\\Windows\\System32"));

			check(
				Text("C:\\Windows\\System32\\"),
				Text("C:\\Windows\\System32"));

			check(
				Text("C:\\Windows"),
				Text("C:\\"));

			check(
				Text("services.exe"),
				Text(""));

			check(
				Text("\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe"),
				Text("\\\\vmware-host\\Shared Folders\\Shared VMs Folder"));

			check(
				Text("\\SystemRoot\\System32\\ntoskrnl.exe"),
				Text("\\SystemRoot\\System32"));

			check(
				Text("\\SystemRoot"),
				Text("\\SystemRoot"));
		}

		TEST_METHOD(SearchFullPath)
		{
			{
				auto fileName = String(Text("cmd.exe"));
				auto fullPath = Path::SearchFullPath(fileName).ToString();
				
				Assert::IsFalse(fullPath.IsEmpty());
				Assert::IsFalse(fullPath == fileName);
				Assert::IsTrue(fullPath.EndsWith(fileName));
			}

			{
				auto fileName = Text("h384hsld0sh3njfd0ijfdd.exe");
				auto fullPath = Path::SearchFullPath(fileName).ToString();
				
				Assert::IsTrue(fullPath == fileName);
			}
		}
	};
}