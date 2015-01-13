#include "Path.h"
#include "..\Environment.h"
#include "..\Buffer.h"

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace WinToolsLib
{
	namespace FileSystem
	{
		Path::Path()
		{
		}

		Path::Path(String&& path) :
			m_path(std::move(path))
		{
		}

		Path::Path(const String& path) :
			m_path(path)
		{
		}

		Path::Path(Path&& other)
		{
			MoveFrom(other);
		}

		Path::Path(const Path& other)
		{
			CopyFrom(other);
		}

		Path& Path::operator=(Path&& other)
		{
			MoveFrom(other);
			return *this;
		}

		Path& Path::operator=(const Path& other)
		{
			CopyFrom(other);
			return *this;
		}

		Path& Path::operator+=(const Path& other)
		{
			Append(other);
			return *this;
		}

		Path Path::operator+(const Path& other)
		{
			Path path(m_path);
			path.Append(other);
			return path;
		}

		Path::operator const String&() const
		{
			return m_path;
		}

		const String& Path::ToString() const
		{
			return m_path;
		}

		Void Path::Append(const String& path)
		{
			if (path.IsEmpty())
			{
				// Nothing to append
				return;
			}

			if (m_path.IsEmpty())
			{
				m_path = path;
				return;
			}

			if (m_path[m_path.GetLength()-1] != '\\')
			{
				if (path[(SizeT)0] != '\\')
				{
					m_path += '\\';
				}
				m_path += path;
			}
			else
			{
				if (path[(SizeT)0] == '\\')
				{
					m_path += path.SubString(1);
				}
				else
				{
					m_path += path;
				}
			}
		}

		Void Path::Append(const Path& other)
		{
			Append(other.m_path);
		}

		Void Path::ReplaceName(const String& name)
		{
			auto slash = m_path.FindLast(Text("\\"));
			auto dot = m_path.FindLast(Text("."));
			
			if (std::string::npos != slash)
			{
				if (std::string::npos != dot)
				{
					m_path.Replace(++slash, dot, name);
				}
				else
				{
					m_path.Replace(++slash, name);
				}
			}
			else
			{
				if (std::string::npos != dot)
				{
					m_path.Replace(0, dot, name);
				}
				else
				{
					m_path = name;
				}
			}
		}

		Void Path::ReplaceFullName(const String& fullName)
		{
			auto slash = m_path.FindLast(Text("\\"));
			if (std::string::npos != slash)
			{
				m_path.Replace(++slash, fullName);
			}
			else
			{
				m_path = fullName;
			}
		}

		Void Path::ReplaceExtension(const String& ext)
		{
			auto dot = m_path.FindLast(Text("."));
			if (std::string::npos != dot)
			{
				m_path.Replace(++dot, ext);
			}
			else
			{
				m_path += Text(".");
				m_path += ext;
			}
		}

		Void Path::ReplaceDirectory(const String& dir)
		{
			auto slash = m_path.FindLast(Text("\\"));
			if (std::string::npos != slash)
			{
				m_path.Replace(0, slash, dir);
			}
			else
			{
				auto temp = m_path;
				m_path = dir;
				Append(temp);
			}
		}

		String Path::GetName() const
		{
			auto slash = m_path.FindLast(Text("\\"));
			if (std::string::npos != slash)
			{
				return m_path.SubString(++slash);
			}
			return m_path;
		}

		String Path::GetNameWithoutExtension() const
		{
			auto dot = m_path.FindLast(Text("."));
			auto slash = m_path.FindLast(Text("\\"));
			if (std::string::npos != slash)
			{
				slash++;
				if (std::string::npos != dot && dot >= slash)
				{
					auto size = dot - slash;
					return m_path.SubString(slash, size);
				}
				else
				{
					return m_path.SubString(slash);
				}
			}
			else
			{
				if (std::string::npos != dot)
				{
					return m_path.SubString(0, dot);
				}
			}
			return m_path;
		}

		String Path::GetFolder() const
		{
			auto slash = m_path.FindLast(Text("\\"));
			if (0 == slash)
			{
				return m_path;
			}

			if (std::string::npos != slash)
			{
				if (m_path[slash - 1] == Text(':'))
				{
					return m_path.SubString(0, slash + 1);
				}
				return m_path.SubString(0, slash);
			}
			return Text("");
		}

		Path Path::UrlToWin32(const TChar* url)
		{
			DWORD length = MAX_PATH + 1;
			TChar win32Path[MAX_PATH + 1] = {0};

			auto hr = ::PathCreateFromUrl(url, win32Path, &length, NULL);
			if (hr != S_OK)
				THROW_WIN32_EXCEPTION(hr);

			return Path(win32Path);
		}

		Path Path::GetFileName(String cmdLine)
		{
			static String rundll(Text("rundll32.exe"));
			
			cmdLine.Trim(Text(" \t"));
			String lowerCopy = static_cast<const String&>(cmdLine).ToLower();
			
			auto dll = lowerCopy.Find(rundll);
			if (-1 != dll)
			{
				auto from = dll + rundll.GetLength() + 1;
				auto comma = cmdLine.Find(Text(","));
				if (-1 != comma)
				{
					auto count = comma - from;
					cmdLine = cmdLine.SubString(from, count);
				}
				else
				{
					cmdLine = cmdLine.SubString(from);
				}
			}

			cmdLine.Trim(Text(" \t"));
			lowerCopy = static_cast<const String&>(cmdLine).ToLower();

			if (cmdLine.BeginsWith(Text(R"(")")))
			{
				WChar** filename = [&cmdLine]()
				{
					Int32 argc;
					return ::CommandLineToArgvW(cmdLine, &argc);
				}();

				if (filename)
				{
					cmdLine = String(filename[0]);
				}
			}

			auto comma = cmdLine.FindLast(Text(","));
			if (-1 != comma)
			{
				cmdLine = cmdLine.SubString(0, comma);
			}

			auto slashes = cmdLine.Find(Text(R"(\\)"));
			while (-1 != slashes && 0 != slashes)
			{
				cmdLine.Replace(slashes, slashes + 2, Text(R"(\)"));
				slashes = cmdLine.Find(Text(R"(\\)"));
			}

			auto exe = lowerCopy.Find(Text(".exe "));
			if (-1 != exe)
			{
				cmdLine = cmdLine.SubString(0, exe + 4);
			}

			if (cmdLine.BeginsWith(Text(R"(\??\)")))
			{
				cmdLine = cmdLine.SubString(4);
			}

			return cmdLine;
		}

		Path Path::NtToWin32(const TChar* ntPath, Bool kernelRules)
		{
			static const auto systemRoot = Environment::ExpandString(Text("%windir%"));
			static const auto systemDrive = Environment::ExpandString(Text("%SystemDrive%"));

			String path(ntPath);
			path.Replace(L"\\SystemRoot", systemRoot);

			if (path.BeginsWith(L"\\??\\"))
			{
				path = path.SubString(4);
			}

			if (kernelRules)
			{
				if (path.BeginsWith(L"\\"))
				{
					path = systemDrive + path;
				}

				auto slash = path.Find(L"\\");
				if (-1 == slash)
				{
					path = systemRoot + Text("\\System32\\Drivers\\") + path;
				}
			}

			return path;
		}

		Path Path::SearchFullPath(const TChar* fileName)
		{
			UInt32 pathLength = ::SearchPath(NULL, fileName, NULL, 0, NULL, NULL);
			if (pathLength > 0)
			{
				String pathStr;
				pathStr.Resize(pathLength - 1);

				if (::SearchPath(NULL, fileName, NULL, pathLength, pathStr.GetBuffer(), NULL) > 0)
				{
					return Path(std::move(pathStr));
				}
			}

			return Path(fileName);
		}

		Path Path::GetFullPath(const TChar* path)
		{
			auto relative = ::PathIsRelative(path);
			if (relative)
			{
				return SearchFullPath(path);
			}

			return Path(path);
		}

		Void Path::MoveFrom(Path& other)
		{
			if (this != &other)
			{
				m_path = std::move(other.m_path);
			}
		}

		Void Path::CopyFrom(const Path& other)
		{
			if (this != &other)
			{
				m_path = other.m_path;
			}
		}
	}	
}