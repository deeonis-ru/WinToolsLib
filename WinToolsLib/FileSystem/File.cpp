#include "File.h"
#include "Path.h"
#include "..\Exception.h"
#include "..\Handles\FindHandle.h"
#include <Windows.h>

namespace WinToolsLib
{
	namespace FileSystem
	{
		File::File()
		{
		}

		File::File(Handle handle, const TChar* path) :
			m_handle(std::move(handle)),
			m_path(path ? path : Text(""))
		{
		}
		
		File::File(File&& other)
		{
			MoveFrom(other);
		}
		
		File::File(const File& other)
		{
			CopyFrom(other);
		}

		File& File::operator=(File&& other)
		{
			MoveFrom(other);
			return *this;
		}

		File& File::operator=(const File& other)
		{
			CopyFrom(other);
			return *this;
		}

		UInt64 File::GetSize() const
		{
			if (!m_handle.IsValid())
			{
				THROW_INVALID_HANDLE_EXCEPTION();
			}

			LARGE_INTEGER fileSize = {0};

			BOOL success = ::GetFileSizeEx(m_handle, &fileSize);
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			return (UInt64)fileSize.QuadPart;
		}

		Buffer File::Read(UInt32 bytes) const
		{
			if (!m_handle.IsValid())
			{
				THROW_INVALID_HANDLE_EXCEPTION();
			}

			auto size = GetSize();
			auto position = GetFilePointer();

			if (0 == bytes)
			{
				bytes = (UInt32)size;
			}
			else if (position + bytes > size)
			{
				bytes = (UInt32)(size - position);
			}

			Buffer buffer(bytes);
			DWORD bytesRead = 0;

			BOOL success = ::ReadFile(
				m_handle,
				buffer.GetBuffer(),
				(DWORD)buffer.GetSize(),
				&bytesRead,
				NULL);

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			return buffer;
		}

		SizeT File::Read(Buffer& buffer) const
		{
			if (!m_handle.IsValid())
			{
				THROW_INVALID_HANDLE_EXCEPTION();
			}

			const auto size = GetSize();
			if (0 == size)
			{
				// Empty file
				return 0;
			}

			auto bytes = buffer.GetSize();
			if (0 == bytes)
			{
				// Buffer is empty, let's allocate enough bytes
				bytes = (SizeT)size;
				buffer = Buffer(bytes);
			}
			else
			{
				const auto position = GetFilePointer();
				if (position + bytes > size)
				{
					bytes = (SizeT)(size - position);
					if (0 == bytes)
					{
						// End of file
						return 0;
					}
				}
			}

			DWORD bytesRead = 0;

			BOOL success = ::ReadFile(
				m_handle,
				buffer.GetBuffer(),
				(DWORD)bytes,
				&bytesRead,
				NULL);

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			return (SizeT)bytesRead;
		}

		Void File::Write(const Buffer& buffer)
		{
			Write(buffer.GetBuffer(), buffer.GetSize());
		}

		Void File::Write(const Byte* buffer, SizeT size)
		{
			if (!m_handle.IsValid())
			{
				THROW_INVALID_HANDLE_EXCEPTION();
			}

			DWORD bytesWritten = 0;

			BOOL success = ::WriteFile(
				m_handle,
				buffer,
				(DWORD)size,
				&bytesWritten,
				NULL);

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		UInt64 File::GetFilePointer() const
		{
			if (!m_handle.IsValid())
			{
				THROW_INVALID_HANDLE_EXCEPTION();
			}

			LARGE_INTEGER distance = {0};

			BOOL success = ::SetFilePointerEx(m_handle, distance, &distance, FILE_CURRENT);
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			return (UInt64)distance.QuadPart;
		}

		Void File::SetFilePointer(UInt64 position)
		{
			if (!m_handle.IsValid())
			{
				THROW_INVALID_HANDLE_EXCEPTION();
			}

			LARGE_INTEGER distance;
			distance.QuadPart = position;

			BOOL success = ::SetFilePointerEx(m_handle, distance, NULL, FILE_BEGIN);
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		Void File::Close()
		{
			m_handle.Close();
		}

		Bool File::Delete()
		{
			Close();
			auto success = Delete(m_path);
			if (success)
			{
				m_path = Text("");
			}
			return success;
		}

		//
		// NOTE: Wouldn't work if handle is open without Share::Delete, meaning in most cases
		//
		//Bool File::Rename(const TChar* newPath)
		//{
		//	auto success = Rename(m_path, newPath);
		//	if (success)
		//	{
		//		m_path = newPath;
		//	}
		//	return success;
		//}

		File File::Open(
			const TChar* path,
			Access   desiredAccess,
			Share    shareMode,
			Creation creationDisposition)
		{
			Handle handle = ::CreateFile(
				path,
				(DWORD)desiredAccess,
				(DWORD)shareMode,
				NULL,
				(DWORD)creationDisposition,
				0,
				NULL);

			if (!handle.IsValid())
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			return File(std::move(handle), path);
		}

		File File::Create(
			const TChar* path,
			Access   desiredAccess,
			Share    shareMode,
			Creation creationDisposition)
		{
			Handle handle = ::CreateFile(
				path,
				(DWORD)desiredAccess,
				(DWORD)shareMode,
				NULL,
				(DWORD)creationDisposition,
				0,
				NULL);

			if (!handle.IsValid())
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			return File(std::move(handle), path);
		}

		File File::CreateTemp(
			const TChar* fileExtension,
			Access   desiredAccess,
			Share    shareMode,
			Creation creationDisposition)
		{
			auto path = GetTempFilePath(fileExtension);
			return File::Create(path, desiredAccess, shareMode, creationDisposition);
		}

		String File::GetTempFilePath(const TChar* fileExtension)
		{
			//
			// TODO: Use dynamic memory allocation here (like string.GetBuffer(MAX_PATH))
			//
			TCHAR pszTemp[MAX_PATH] = {0};

			BOOL success = ::GetTempPath(MAX_PATH, pszTemp);
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
			
			//
			// TODO: Use dynamic memory allocation here (like string.GetBuffer(MAX_PATH))
			//
			TCHAR pszFile[MAX_PATH] = {0};

			success = ::GetTempFileName(pszTemp, NULL, 0, pszFile);
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}

			//
			// NOTE: Function GetTempFileName actually creates file
			//
			::DeleteFile(pszFile);

			Path path(pszFile);
			if (nullptr != fileExtension)
			{
				path.ReplaceExtension(fileExtension);
			}

			return path.ToString();
		}
		
		Bool File::Rename(const TChar* oldPath, const TChar* newPath)
		{
			return TRUE == ::MoveFileEx(oldPath, newPath, MOVEFILE_REPLACE_EXISTING);
		}

		Bool File::Copy(const TChar* srcPath, const TChar* destPath)
		{
			return TRUE == ::CopyFile(srcPath, destPath, FALSE);
		}

		Bool File::Delete(const TChar* path)
		{
			return TRUE == ::DeleteFile(path);
		}

		Bool File::DeleteOnReboot(const TChar* path)
		{
			return TRUE == ::MoveFileEx(path, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		}

		Bool File::Exists(const TChar* path)
		{
			WIN32_FIND_DATA data;
			FindHandle handle = ::FindFirstFile(path, &data);
			const auto error = ::GetLastError();

			if (handle.IsValid())
			{
				return True;
			}

			if (ERROR_FILE_NOT_FOUND == error ||
				ERROR_PATH_NOT_FOUND == error)
			{
				return False;
			}

			return True;
		}

		UInt32 File::GetAttributes(const TChar* path)
		{
			const auto attributes = ::GetFileAttributes(path);
			if (INVALID_FILE_ATTRIBUTES == attributes)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
			return attributes;
		}

		Void File::SetAttributes(const TChar* path, UInt32 attributes)
		{
			const auto success = ::SetFileAttributes(path, attributes);
			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		Void File::MoveFrom(File& other)
		{
			if (this != &other)
			{
				m_handle = std::move(other.m_handle);
				m_path = std::move(other.m_path);
			}
		}

		Void File::CopyFrom(const File& other)
		{
			if (this != &other)
			{
				m_handle = other.m_handle;
				m_path = other.m_path;
			}
		}
	}
}