#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Time.h"
#include "..\..\Registry\Key.h"
#include "..\StartupLocation.h"

namespace WinToolsLib { namespace Os { namespace MsConfig
{
	class StartupFolder
	{
	public:
		StartupFolder();

		StartupFolder(StartupFolder&& other);
		StartupFolder& operator=(StartupFolder&& other);

		StartupFolder(const StartupFolder& other);
		StartupFolder& operator=(const StartupFolder& other);

		const String& GetBackup() const;
		const String& GetBackupExtension() const;
		const String& GetCommand() const;
		const String& GetItem() const;
		const String& GetLocation() const;
		const String& GetPath() const;
		const Time& GetDisabledTime() const;

		Void SetBackup(String backup);
		Void SetBackupExtension(String backupExtension);
		Void SetCommand(String command);
		Void SetItem(String item);
		Void SetLocation(String location);
		Void SetPath(String path);
		Void SetDisabledTime(Time disabledTime);

		Void Read(const Registry::Key& key);
		Void Write(Registry::Key& key) const;
		
		static StartupFolder DisableEntry(
			const TChar* path,
			const TChar* command,
			Startup::FileLocation location);

		static Bool EnableEntry(const TChar* path);
		static Bool DeleteEntry(const TChar* path);

		typedef std::function<Void(const StartupFolder& entry)> EnumCallback;
		static Void EnumEntries(EnumCallback callback);

		static const TChar* GetBackupExtension(Startup::FileLocation location);
		static String GetBackupFolder();
		static String GetBackupPath(const TChar* fileName, Startup::FileLocation location);

	protected:
		Void MoveFrom(StartupFolder& other);
		Void CopyFrom(const StartupFolder& other);

	private:
		String m_backup;
		String m_backupExtension;
		String m_command;
		String m_item;
		String m_location;
		String m_path;
		Time m_disabledTime;
	};

	inline const String& StartupFolder::GetBackup() const
	{
		return m_backup;
	}

	inline const String& StartupFolder::GetBackupExtension() const
	{
		return m_backupExtension;
	}

	inline const String& StartupFolder::GetCommand() const
	{
		return m_command;
	}

	inline const String& StartupFolder::GetItem() const
	{
		return m_item;
	}

	inline const String& StartupFolder::GetLocation() const
	{
		return m_location;
	}

	inline const String& StartupFolder::GetPath() const
	{
		return m_path;
	}

	inline const Time& StartupFolder::GetDisabledTime() const
	{
		return m_disabledTime;
	}

	inline Void StartupFolder::SetBackup(String backup)
	{
		m_backup = std::move(backup);
	}

	inline Void StartupFolder::SetBackupExtension(String backupExtension)
	{
		m_backupExtension = std::move(backupExtension);
	}

	inline Void StartupFolder::SetCommand(String command)
	{
		m_command = std::move(command);
	}

	inline Void StartupFolder::SetItem(String item)
	{
		m_item = std::move(item);
	}

	inline Void StartupFolder::SetLocation(String location)
	{
		m_location = std::move(location);
	}

	inline Void StartupFolder::SetPath(String path)
	{
		m_path = std::move(path);
	}

	inline Void StartupFolder::SetDisabledTime(Time disabledTime)
	{
		m_disabledTime = std::move(disabledTime);
	}

} } }