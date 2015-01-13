#pragma once
#include "..\String.h"

namespace WinToolsLib
{
	namespace FileSystem
	{
		class Shortcut
		{
		public:
			static StringW Resolve(const TChar* shortcutPath);
			static Bool IsShortcut(const String& fileName);
			static Void Create(const WChar* shortcutPath, const WChar* targetPath);
		};
	}
}