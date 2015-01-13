#include "Folder.h"
#include "Path.h"

#include "..\Handles\FindHandle.h"

#include <tchar.h>

namespace WinToolsLib
{
	namespace FileSystem
	{
		using namespace Handles;

		Void Folder::EnumFiles(
			const String& path,
			EnumCallback fileEnumCallback,
			EnumCallback folderEnumCallback)
		{
			Path searchDir(path);
			searchDir.Append(Text("*"));

			WIN32_FIND_DATA data;
			FindHandle handle = ::FindFirstFile(searchDir.ToString(), &data);
			if (!handle.IsValid()) 
			{
				return;
			}

			do 
			{
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					if (_tcscmp(data.cFileName, Text(".")) != 0 &&
						_tcscmp(data.cFileName, Text("..")) != 0)
					{
						folderEnumCallback(&data);
					}
				} 
				else
				{
					fileEnumCallback(&data);
				}

			} while(::FindNextFile(handle, &data));
		}

		Bool Folder::Create(const TChar* path)
		{
			auto success = ::CreateDirectory(path, NULL);
			if (!success)
			{
				auto error = ::GetLastError();
				if (ERROR_ALREADY_EXISTS != error)
				{
					return False;
				}
			}
			return True;
		}
	}
}