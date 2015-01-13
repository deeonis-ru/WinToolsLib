#pragma once
#include "..\Types.h"
#include "..\Buffer.h"
#include "..\String.h"
#include "..\Handles\Handle.h"

namespace WinToolsLib
{
	namespace FileSystem
	{
		using namespace Handles;

		enum class Access : UInt32
		{
			Read    = 0x80000000,
			Write   = 0x40000000,
			Execute = 0x20000000,
			All     = 0x10000000
		};

		enum class Share : UInt32
		{
			Read   = 0x00000001,
			Write  = 0x00000002,
			Delete = 0x00000004
		};

		enum class Creation : UInt32
		{
			CreateNew        = 1,
			CreateAlways     = 2,
			OpenExisting     = 3,
			OpenAlways       = 4,
			TruncateExisting = 5
		};

		class Attribute
		{
			enum : UInt32
			{
				ReadOnly          = 0x00000001,
				Hidden            = 0x00000002,
				System            = 0x00000004,
				Directory         = 0x00000010,
				Archive           = 0x00000020,
				Device            = 0x00000040,
				Normal            = 0x00000080,
				Temporary         = 0x00000100,
				SparseFile        = 0x00000200,
				ReparsePoint      = 0x00000400,
				Compressed        = 0x00000800,
				Offline           = 0x00001000,
				NotContentIndexed = 0x00002000,
				Encrypted         = 0x00004000,
				Virtual           = 0x00010000
			};
		};

		class File
		{
		public:
			File();
			File(Handle handle, const TChar* path = nullptr);
			File(File&& other);
			File(const File& other);

			File& operator=(File&& other);
			File& operator=(const File& other);

			UInt64 GetSize() const;
			Buffer Read(UInt32 bytes = 0) const;
			SizeT Read(Buffer& buffer) const;

			Void Write(const Buffer& buffer);
			Void Write(const Byte* buffer, SizeT size);

			UInt64 GetFilePointer() const;
			Void SetFilePointer(UInt64 position);

			const String& GetPath() const;
			
			Void Close();
			Bool Delete();

			//
			// NOTE: Wouldn't work if handle is open without Share::Delete, meaning in most cases
			//
			//Bool Rename(const TChar* newPath);
			
			static File Open(
				const TChar* path,
				Access   desiredAccess       = Access::Read,
				Share    shareMode           = Share::Read,
				Creation creationDisposition = Creation::OpenExisting);
			
			static File Create(
				const TChar* path,
				Access   desiredAccess       = Access::Write,
				Share    shareMode           = Share::Read,
				Creation creationDisposition = Creation::CreateAlways);

			static File CreateTemp(
				const TChar* fileExtension   = nullptr,
				Access   desiredAccess       = Access::Write,
				Share    shareMode           = Share::Read,
				Creation creationDisposition = Creation::CreateAlways);

			static String GetTempFilePath(const TChar* fileExtension = nullptr);

			static Bool Rename(const TChar* oldPath, const TChar* newPath);
			static Bool Copy(const TChar* srcPath, const TChar* destPath);
			
			static Bool Delete(const TChar* path);
			static Bool DeleteOnReboot(const TChar* path);
			static Bool Exists(const TChar* path);

			static UInt32 GetAttributes(const TChar* path);
			static Void SetAttributes(const TChar* path, UInt32 attributes);

		protected:
			Void MoveFrom(File& other);
			Void CopyFrom(const File& other);

		private:
			Handle m_handle;
			String m_path;
		};

		inline const String& File::GetPath() const
		{
			return m_path;
		}
	}
}