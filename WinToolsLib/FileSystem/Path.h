#pragma once
#include "..\Types.h"
#include "..\String.h"

namespace WinToolsLib
{
	namespace FileSystem
	{
		class Path
		{
		public:
			Path();
			Path(String&& path);
			Path(const String& path);
			Path(Path&& other);
			Path(const Path& other);

			Path& operator=(Path&& other);
			Path& operator=(const Path& other);

			Path& operator+=(const Path& other);
			Path operator+(const Path& other);

			operator const String&() const;
			const String& ToString() const;

			Void Append(const String& path);
			Void Append(const Path& other);

			Void ReplaceName(const String& other);
			Void ReplaceFullName(const String& other);
			Void ReplaceExtension(const String& other);
			Void ReplaceDirectory(const String& other);

			String GetName() const;
			String GetNameWithoutExtension() const;
			String GetFolder() const;

			static Path UrlToWin32(const TChar* url);
			static Path GetFileName(String cmdLine);
			static Path NtToWin32(const TChar* ntPath, Bool kernelRules);
			static Path SearchFullPath(const TChar* fileName);
			static Path GetFullPath(const TChar* path);

		protected:
			Void MoveFrom(Path& other);
			Void CopyFrom(const Path& other);

		private:
			String m_path;
		};
	}
}