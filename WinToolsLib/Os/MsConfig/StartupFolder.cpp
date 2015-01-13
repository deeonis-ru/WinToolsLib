#include "StartupFolder.h"

#include "..\Shell.h"
#include "..\..\System.h"
#include "..\..\Trace.h"
#include "..\..\FileSystem\File.h"
#include "..\..\FileSystem\Folder.h"
#include "..\..\FileSystem\Path.h"

namespace WinToolsLib { namespace Os { namespace MsConfig
{
	using namespace Registry;
	
	StartupFolder::StartupFolder()
	{
	}

	StartupFolder::StartupFolder(StartupFolder&& other)
	{
		MoveFrom(other);
	}

	StartupFolder& StartupFolder::operator=(StartupFolder&& other)
	{
		MoveFrom(other);
		return *this;
	}

	StartupFolder::StartupFolder(const StartupFolder& other)
	{
		CopyFrom(other);
	}

	StartupFolder& StartupFolder::operator=(const StartupFolder& other)
	{
		CopyFrom(other);
		return *this;
	}

	Void StartupFolder::Read(const Registry::Key& key)
	{
		Buffer data;
				
		key.GetValue(Text("backup"), ValueType::String, data);
		m_backup = (TChar*)data.GetBuffer();
				
		key.GetValue(Text("command"), ValueType::String, data);
		m_command = (TChar*)data.GetBuffer();
				
		key.GetValue(Text("item"), ValueType::String, data);
		m_item = (TChar*)data.GetBuffer();
				
		key.GetValue(Text("location"), ValueType::String, data);
		m_location = (TChar*)data.GetBuffer();
				
		key.GetValue(Text("path"), ValueType::String, data);
		m_path = (TChar*)data.GetBuffer();
				
		if (System::IsVistaOrLater())
		{
			key.GetValue(Text("backupExtension"), ValueType::String, data);
			m_backupExtension = (TChar*)data.GetBuffer();

			key.GetValue(Text("DAY"), ValueType::DoubleWord, data);
			auto day = *(UInt16*)data.GetBuffer();
				
			key.GetValue(Text("HOUR"), ValueType::DoubleWord, data);
			auto hour = *(UInt16*)data.GetBuffer();
				
			key.GetValue(Text("MINUTE"), ValueType::DoubleWord, data);
			auto minute = *(UInt16*)data.GetBuffer();
				
			key.GetValue(Text("MONTH"), ValueType::DoubleWord, data);
			auto month = *(UInt16*)data.GetBuffer();
				
			key.GetValue(Text("SECOND"), ValueType::DoubleWord, data);
			auto second = *(UInt16*)data.GetBuffer();
				
			key.GetValue(Text("YEAR"), ValueType::DoubleWord, data);
			auto year = *(UInt16*)data.GetBuffer();

			m_disabledTime = Time(year, month, day, hour, minute, second);
		}
	}

	Void StartupFolder::Write(Registry::Key& key) const
	{
		auto setStringValue = [&key](const TChar* property, const String& value)
		{
			key.SetValue(property, ValueType::String, (PByte)value.GetBuffer(), (UInt32)value.GetLength() * sizeof(TChar));
		};

		auto setDwordValue = [&key](const TChar* property, UInt32 value)
		{
			key.SetValue(property, ValueType::DoubleWord, (PByte)&value, (UInt32)sizeof(UInt32));
		};

		setStringValue(Text("backup"), m_backup);
		setStringValue(Text("command"), m_command);
		setStringValue(Text("item"), m_item);
		setStringValue(Text("location"), m_location);
		setStringValue(Text("path"), m_path);
				
		if (System::IsVistaOrLater())
		{
			setStringValue(Text("backupExtension"), m_backupExtension);
			setDwordValue(Text("DAY"), m_disabledTime.GetDay());
			setDwordValue(Text("HOUR"), m_disabledTime.GetHour());
			setDwordValue(Text("MINUTE"), m_disabledTime.GetMinute());
			setDwordValue(Text("MONTH"), m_disabledTime.GetMonth());
			setDwordValue(Text("SECOND"), m_disabledTime.GetSecond());
			setDwordValue(Text("YEAR"), m_disabledTime.GetYear());
		}
	}

	StartupFolder StartupFolder::DisableEntry(
		const TChar* path,
		const TChar* command,
		Startup::FileLocation location)
	{
		auto backupExntension = GetBackupExtension(location);
		if (!backupExntension)
		{
			THROW_UNSUPPORTED_LOCATION();
		}

		if (!FileSystem::File::Exists(path))
		{
			THROW_FILE_NOT_FOUND_EXCEPTION(path);
		}

		String keyName(path);
		keyName.Replace(Text("\\"), Text("^"));

		auto key = Key::Open(
			HKEY_LOCAL_MACHINE,
			Text("SOFTWARE\\Microsoft\\Shared Tools"),
			KEY_CREATE_SUB_KEY);

		key = Key::Create(key, Text("MSConfig"), KEY_CREATE_SUB_KEY);
		key = Key::Create(key, Text("startupfolder"), KEY_CREATE_SUB_KEY);
		key = Key::Create(key, keyName, KEY_SET_VALUE);

		auto filePath = FileSystem::Path(path);
		auto fileFolder = filePath.GetFolder();
		auto backupPath = GetBackupPath(filePath.GetName(), location);

		StartupFolder entry;
		entry.SetBackup(backupPath);
		entry.SetCommand(command);
		entry.SetItem(filePath.GetNameWithoutExtension());
		entry.SetPath(path);

		if (System::IsVistaOrLater())
		{
			entry.SetLocation(fileFolder);
			entry.SetBackupExtension(backupExntension);
			entry.SetDisabledTime(Time::GetSystemTime().ToLocal());
		}
		else
		{
			entry.SetLocation(backupExntension);
		}

		entry.Write(key);

		auto backupFolder = GetBackupFolder();
		if (!FileSystem::File::Exists(backupFolder))
		{
			FileSystem::Folder::Create(backupFolder);
		}

		if (!FileSystem::File::Rename(path, backupPath))
		{
			auto lastError = ::GetLastError();
			auto pathCopy = String(path);
			TRACE_ERROR(L"Can't move file %s to %s", pathCopy.GetBuffer(), backupPath.GetBuffer());
			if (FileSystem::File::Exists(path))
			{
				DeleteEntry(path);
			}
			THROW_WIN32_EXCEPTION(lastError);
		}
		return entry;
	}

	Bool StartupFolder::EnableEntry(const TChar* path)
	{
		String name(path);
		name.Replace(Text("\\"), Text("^"));

		auto key = Key::Open(
			HKEY_LOCAL_MACHINE,
			Text("SOFTWARE\\Microsoft\\Shared Tools\\MSConfig\\startupfolder"),
			KEY_ALL_ACCESS);

		auto readBackupPath = [&]()
		{
			auto subKey = Key::Open(key, name, KEY_QUERY_VALUE);

			StartupFolder entry;
			entry.Read(subKey);
			return entry.GetBackup();
		};
			
		if (Key::Exists(key, name))
		{
			auto backupPath = readBackupPath();
			if (!FileSystem::File::Rename(backupPath, path))
			{
				String pathCopy = path;
				TRACE_ERROR(L"Can't move file %s to %s", backupPath.GetBuffer(), pathCopy.GetBuffer());
				return False;
			}

			auto success = Key::Delete(key, name);
			if (!success || Key::Exists(key, name))
			{
				TRACE_ERROR(L"Can't delete MSConfig\\startupfolder\\%s", name.GetBuffer());
				FileSystem::File::Rename(path, backupPath);
				return False;
			}
		}
		return True;
	}

	Bool StartupFolder::DeleteEntry(const TChar* path)
	{
		String name(path);
		name.Replace(Text("\\"), Text("^"));

		auto key = Key::Open(
			HKEY_LOCAL_MACHINE,
			Text("SOFTWARE\\Microsoft\\Shared Tools\\MSConfig\\startupfolder"),
			KEY_ALL_ACCESS);
			
		if (Key::Exists(key, name))
		{
			auto success = Key::Delete(key, name);
			if (!success || Key::Exists(key, name))
			{
				TRACE_ERROR(L"Can't delete MSConfig\\startupfolder\\%s", name.GetBuffer());
				return False;
			}
		}
		return True;
	}

	Void StartupFolder::EnumEntries(EnumCallback callback)
	{
		try
		{
			auto key = Key::Open(
				HKEY_LOCAL_MACHINE,
				Text("SOFTWARE\\Microsoft\\Shared Tools\\MSConfig\\startupfolder"),
				KEY_ENUMERATE_SUB_KEYS);

			auto subKeyCallback = [&callback](const TChar* name, Key& key)
			{
				StartupFolder entry;
				entry.Read(key);
				callback(entry);
			};
			key.EnumSubKey(subKeyCallback, KEY_QUERY_VALUE);
		}
		catch (FileNotFoundException&)
		{
			// It's OK if MSConfig key is absent.
		}
	}

	const TChar* StartupFolder::GetBackupExtension(Startup::FileLocation location)
	{
		switch (location)
		{
		case Startup::FileLocation::CommonStartup:
			{
				if (System::IsVistaOrLater())
				{
					return Text(".CommonStartup");
				}
				else
				{
					return Text("Common Startup");
				}
				break;
			}

		case Startup::FileLocation::UserStartup:
			{
				if (System::IsVistaOrLater())
				{
					return Text(".Startup");
				}
				else
				{
					return Text("Startup");
				}
				break;
			}

		default:
			return nullptr;
		}
	}

	String StartupFolder::GetBackupFolder()
	{
		FileSystem::Path folder = Shell::GetKnownFolder(KnownFolder::Windows);
		folder.Append(Text("pss"));
		return folder.ToString();
	}

	String StartupFolder::GetBackupPath(const TChar* fileName, Startup::FileLocation location)
	{
		auto folder = GetBackupFolder();
		auto extension = GetBackupExtension(location);
		
		return String::FormatS(
			Text("%s\\%s%s"),
			folder.GetBuffer(),
			fileName,
			extension);
	}

	Void StartupFolder::MoveFrom(StartupFolder& other)
	{
		if (this != &other)
		{
			m_backup = std::move(other.m_backup);
			m_backupExtension = std::move(other.m_backupExtension);
			m_command = std::move(other.m_command);
			m_item = std::move(other.m_item);
			m_location = std::move(other.m_location);
			m_path = std::move(other.m_path);
			m_disabledTime = std::move(other.m_disabledTime);
		}
	}

	Void StartupFolder::CopyFrom(const StartupFolder& other)
	{
		if (this != &other)
		{
			m_backup = other.m_backup;
			m_backupExtension = other.m_backupExtension;
			m_command = other.m_command;
			m_item = other.m_item;
			m_location = other.m_location;
			m_path = other.m_path;
			m_disabledTime = other.m_disabledTime;
		}
	}

} } }