#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Time.h"
#include "..\..\Registry\Key.h"
#include "..\StartupLocation.h"

#include <functional>
#include <Windows.h>

namespace WinToolsLib { namespace Os { namespace MsConfig
{
	class StartupRegistry
	{
		StartupRegistry();

	public:
		StartupRegistry(StartupRegistry&& other);
		StartupRegistry& operator=(StartupRegistry&& other);

		StartupRegistry(const StartupRegistry& other);
		StartupRegistry& operator=(const StartupRegistry& other);
		
		HKEY GetRoot() const;
		const String& GetCommand() const;
		const String& GetItem() const;
		const String& GetKey() const;
		const Time& GetDisabledTime() const;
		const String GetOffPath() const;

		Void SetRoot(HKEY root);
		Void SetCommand(const TChar* command);
		Void SetItem(const TChar* item);
		Void SetKey(const TChar* key);
		Void SetDisabledTime(Time disabledTime);

		Void Read(const Registry::Key& key);
		Void Write(Registry::Key& key) const;

		static StartupRegistry CreateEntry(
			HKEY root,
			const TChar* path,
			const TChar* name,
			const TChar* command,
			Startup::RegistryLocation location);

		static Bool DeleteEntry(const TChar* name);

		typedef std::function<Void(const StartupRegistry& entry)> EnumCallback;
		static Void EnumEntries(EnumCallback callback);

	protected:
		Void MoveFrom(StartupRegistry& other);
		Void CopyFrom(const StartupRegistry& other);

	private:
		HKEY   m_root;
		String m_command;
		String m_item;
		String m_key;
		Time   m_disabledTime;
	};

	inline HKEY StartupRegistry::GetRoot() const
	{
		return m_root;
	}

	inline const String& StartupRegistry::GetCommand() const
	{
		return m_command;
	}

	inline const String& StartupRegistry::GetItem() const
	{
		return m_item;
	}
	
	inline const String& StartupRegistry::GetKey() const
	{
		return m_key;
	}
	
	inline const Time& StartupRegistry::GetDisabledTime() const
	{
		return m_disabledTime;
	}

	inline const String StartupRegistry::GetOffPath() const
	{
		return String::FormatS(Text("SOFTWARE\\Microsoft\\Shared Tools\\MSConfig\\startupreg\\%s"), m_item.GetBuffer());
	}

	inline Void StartupRegistry::SetRoot(HKEY root)
	{
		m_root = root;
	}

	inline Void StartupRegistry::SetCommand(const TChar* command)
	{
		m_command = command;
	}

	inline Void StartupRegistry::SetItem(const TChar* item)
	{
		m_item = item;
	}

	inline Void StartupRegistry::SetKey(const TChar* key)
	{
		m_key = key;
	}

	inline Void StartupRegistry::SetDisabledTime(Time disabledTime)
	{
		m_disabledTime = disabledTime;
	}
} } }