#pragma once
#include "..\Types.h"
#include "..\String.h"
#include <functional>
#include <Windows.h>

namespace WinToolsLib
{
	namespace FileSystem
	{
		class Folder
		{
		public:
			typedef std::function<Void(LPWIN32_FIND_DATA)> EnumCallback;

			static Void EnumFiles(
				const String& path,
				EnumCallback fileEnumCallback,
				EnumCallback folderEnumCallback);

			static Bool Create(const TChar* path);
		};
	}
}