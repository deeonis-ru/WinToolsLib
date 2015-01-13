#include "StartupRegistry.h"

#include "..\..\Buffer.h"
#include "..\..\Trace.h"
#include "..\..\System.h"
#include "..\..\FileSystem\Path.h"

namespace WinToolsLib { namespace Os { namespace MsConfig
{
	using namespace Registry;
	
	StartupRegistry::StartupRegistry() :
		m_root(nullptr)
	{
	}

	StartupRegistry::StartupRegistry(StartupRegistry&& other)
	{
		MoveFrom(other);
	}

	StartupRegistry& StartupRegistry::operator=(StartupRegistry&& other)
	{
		MoveFrom(other);
		return *this;
	}

	StartupRegistry::StartupRegistry(const StartupRegistry& other)
	{
		CopyFrom(other);
	}

	StartupRegistry& StartupRegistry::operator=(const StartupRegistry& other)
	{
		CopyFrom(other);
		return *this;
	}

	Void StartupRegistry::Read(const Registry::Key& key)
	{
		Buffer data;
				
		key.GetValue(Text("command"), ValueType::String, data);
		m_command = data.GetBuffer() ? (TChar*)data.GetBuffer() : Text("");
				
		key.GetValue(Text("hkey"), ValueType::String, data);
		m_root = Key::StringToRootHkey((TChar*)data.GetBuffer());
				
		key.GetValue(Text("item"), ValueType::String, data);
		m_item = (TChar*)data.GetBuffer();
				
		key.GetValue(Text("key"), ValueType::String, data);
		m_key = (TChar*)data.GetBuffer();
				
		if (System::IsVistaOrLater())
		{
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

	Void StartupRegistry::Write(Registry::Key& key) const
	{
		auto setStringValue = [&key](const TChar* property, const String& value)
		{
			key.SetValue(property, ValueType::String, (PByte)value.GetBuffer(), (UInt32)value.GetLength() * sizeof(TChar));
		};

		auto setDwordValue = [&key](const TChar* property, UInt32 value)
		{
			key.SetValue(property, ValueType::DoubleWord, (PByte)&value, (UInt32)sizeof(UInt32));
		};

		setStringValue(Text("command"), m_command);
		setStringValue(Text("hkey"), Key::RootToString(m_root));
		setStringValue(Text("inimapping"), Text("0"));
		setStringValue(Text("item"), m_item);
		setStringValue(Text("key"), m_key);
				
		if (System::IsVistaOrLater())
		{
			setDwordValue(Text("DAY"), m_disabledTime.GetDay());
			setDwordValue(Text("HOUR"), m_disabledTime.GetHour());
			setDwordValue(Text("MINUTE"), m_disabledTime.GetMinute());
			setDwordValue(Text("MONTH"), m_disabledTime.GetMonth());
			setDwordValue(Text("SECOND"), m_disabledTime.GetSecond());
			setDwordValue(Text("YEAR"), m_disabledTime.GetYear());
		}
	}

	StartupRegistry StartupRegistry::CreateEntry(
		HKEY root,
		const TChar* path,
		const TChar* name,
		const TChar* command,
		Startup::RegistryLocation location)
	{
		if (Startup::RegistryLocation::CurrentUserRun != location &&
			Startup::RegistryLocation::LocalMachineRun != location &&
			Startup::RegistryLocation::LocalMachineRun32 != location)
		{
			THROW_UNSUPPORTED_LOCATION();
		}

		auto key = Key::Open(
			HKEY_LOCAL_MACHINE,
			Text("SOFTWARE\\Microsoft\\Shared Tools"),
			KEY_CREATE_SUB_KEY);

		key = Key::Create(key, Text("MSConfig"), KEY_CREATE_SUB_KEY);
		key = Key::Create(key, Text("startupreg"), KEY_CREATE_SUB_KEY);
		key = Key::Create(key, name, KEY_SET_VALUE);

		StartupRegistry entry;
		entry.SetCommand(command);
		entry.SetRoot(root);
		entry.SetKey(path);

		if (System::IsVistaOrLater())
		{
			entry.SetItem(name);
			entry.SetDisabledTime(Time::GetSystemTime().ToLocal());
		}
		else
		{
			auto filePath = FileSystem::Path::GetFileName(command);
			auto fileName = filePath.GetNameWithoutExtension();
			entry.SetItem(fileName);
		}

		entry.Write(key);
		return entry;
	}

	Bool StartupRegistry::DeleteEntry(const TChar* name)
	{
		auto key = Key::Open(
			HKEY_LOCAL_MACHINE,
			Text("SOFTWARE\\Microsoft\\Shared Tools\\MSConfig\\startupreg"),
			KEY_ALL_ACCESS);
			
		if (Key::Exists(key, name))
		{
			auto success = Key::Delete(key, name);
			if (!success || Key::Exists(key, name))
			{
				String nameCopy = name;
				TRACE_ERROR(L"Can't delete MSConfig\\startupreg\\%s", nameCopy.GetBuffer());
				return False;
			}
		}
		return True;
	}

	Void StartupRegistry::EnumEntries(EnumCallback callback)
	{
		try
		{
			auto key = Key::Open(
				HKEY_LOCAL_MACHINE,
				Text("SOFTWARE\\Microsoft\\Shared Tools\\MSConfig\\startupreg"),
				KEY_ENUMERATE_SUB_KEYS);

			auto subKeyCallback = [&callback](const TChar* name, Key& key)
			{
				StartupRegistry entry;
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
	
	Void StartupRegistry::MoveFrom(StartupRegistry& other)
	{
		if (this != &other)
		{
			m_root = other.m_root;
			m_command = std::move(other.m_command);
			m_item = std::move(other.m_item);
			m_key = std::move(other.m_key);
			m_disabledTime = std::move(other.m_disabledTime);

			other.m_root = nullptr;
		}
	}

	Void StartupRegistry::CopyFrom(const StartupRegistry& other)
	{
		if (this != &other)
		{
			m_root = other.m_root;
			m_command = other.m_command;
			m_item = other.m_item;
			m_key = other.m_key;
			m_disabledTime = other.m_disabledTime;
		}
	}

} } }