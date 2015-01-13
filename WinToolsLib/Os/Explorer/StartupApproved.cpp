#include "StartupApproved.h"

#include "..\..\Buffer.h"
#include "..\..\Stream.h"
#include "..\..\Trace.h"
#include "..\..\FileSystem\Path.h"

namespace WinToolsLib { namespace Os { namespace Explorer
{
	using namespace FileSystem;
	using namespace Registry;

	StartupApproved::StartupApproved() :
		m_state(State::Unknown)
	{
	}
	
	StartupApproved::StartupApproved(StartupApproved&& other)
	{
		MoveFrom(other);
	}

	StartupApproved& StartupApproved::operator=(StartupApproved&& other)
	{
		MoveFrom(other);
		return *this;
	}

	StartupApproved::StartupApproved(const StartupApproved& other)
	{
		CopyFrom(other);
	}

	StartupApproved& StartupApproved::operator=(const StartupApproved& other)
	{
		CopyFrom(other);
		return *this;
	}

	Bool StartupApproved::EnableEntry(const TChar* path, Startup::FileLocation location)
	{
		auto root = GetRoot(location);
		if (!root)
		{
			return False;
		}

		auto name = Path(path).GetName();
		auto key = Key::Open(
			root,
			GetKeyPath(location),
			KEY_SET_VALUE);

		StartupApproved entry;
		entry.SetState(State::Enabled);
		entry.Write(name, key);
		return True;
	}

	Bool StartupApproved::EnableEntry(const TChar* name, Startup::RegistryLocation location)
	{
		auto root = GetRoot(location);
		if (!root)
		{
			return False;
		}

		auto key = Key::Open(
			root,
			GetKeyPath(location),
			KEY_SET_VALUE);

		StartupApproved entry;
		entry.SetState(State::Enabled);
		entry.Write(name, key);
		return True;
	}

	Bool StartupApproved::DisableEntry(const TChar* path, Startup::FileLocation location)
	{
		auto root = GetRoot(location);
		if (!root)
		{
			return False;
		}

		auto name = Path(path).GetName();
		auto key = Key::Open(
			root,
			GetKeyPath(location),
			KEY_SET_VALUE);

		StartupApproved entry;
		entry.SetState(State::Disabled);
		entry.SetDisabledTime(Time::GetSystemTime());
		entry.Write(name, key);
		return True;
	}

	Bool StartupApproved::DisableEntry(const TChar* name, Startup::RegistryLocation location)
	{
		auto root = GetRoot(location);
		if (!root)
		{
			return False;
		}

		auto key = Key::Open(
			root,
			GetKeyPath(location),
			KEY_SET_VALUE);

		StartupApproved entry;
		entry.SetState(State::Disabled);
		entry.SetDisabledTime(Time::GetSystemTime());
		entry.Write(name, key);
		return True;
	}

	Bool StartupApproved::DeleteEntry(const TChar* path, Startup::FileLocation location)
	{
		auto root = GetRoot(location);
		if (!root)
		{
			return False;
		}

		auto name = Path(path).GetName();
		auto key = Key::Open(
			root,
			GetKeyPath(location),
			KEY_ALL_ACCESS);
			
		if (Key::Exists(key, name))
		{
			auto success = Key::Delete(key, name);
			if (!success || Key::Exists(key, name))
			{
				TRACE_ERROR(L"Can't delete StartupApproved file entry %s", name.GetBuffer());
				return False;
			}
		}
		return True;
	}

	Bool StartupApproved::DeleteEntry(const TChar* name, Startup::RegistryLocation location)
	{
		auto root = GetRoot(location);
		if (!root)
		{
			return False;
		}

		auto path = GetKeyPath(location);
		auto key = Key::Open(root, path, KEY_ALL_ACCESS);

		if (Key::Exists(key, name))
		{
			auto success = Key::Delete(key, name);
			if (!success || Key::Exists(key, name))
			{
				String nameCopy = name;
				TRACE_ERROR(L"Can't delete StartupApproved registry entry %s", nameCopy.GetBuffer());
				return False;
			}
		}
		return True;
	}

	StartupApproved StartupApproved::ReadEntry(const TChar* path, Startup::FileLocation location)
	{
		try
		{
			auto root = GetRoot(location);
			if (!root)
			{
				return StartupApproved();
			}

			auto name = Path(path).GetName();
			auto key = Key::Open(
				root,
				GetKeyPath(location),
				KEY_QUERY_VALUE);

			StartupApproved entry;
			entry.Read(name, key);
			return entry;
		}
		catch (...)
		{
			return StartupApproved();
		}
	}

	StartupApproved StartupApproved::ReadEntry(const TChar* name, Startup::RegistryLocation location)
	{
		try
		{
			auto root = GetRoot(location);
			if (!root)
			{
				return StartupApproved();
			}

			auto key = Key::Open(
				root,
				GetKeyPath(location),
				KEY_QUERY_VALUE);
			
			StartupApproved entry;
			entry.Read(name, key);
			return entry;
		}
		catch (...)
		{
			return StartupApproved();
		}
	}

	Bool StartupApproved::Read(const TChar* name, const Registry::Key& key)
	{
		Buffer data;
		Stream stream(data);
		FILETIME fileTime = {0};

		auto success = key.GetValue(name, ValueType::Binary, data);
		if (!success)
		{
			return False;
		}
		
		stream.Read((PByte)&m_state, sizeof(m_state));
		stream.Read((PByte)&fileTime, sizeof(fileTime));

		m_disabledTime = Time::FromFileTime(&fileTime);
		return True;
	}

	Bool StartupApproved::Write(const TChar* name, Registry::Key& key) const
	{
		Buffer data(sizeof(m_state) + sizeof(FILETIME));
		Stream stream(data);

		FILETIME fileTime = {0};

		// Do not write disabledTime for enabled entries!
		if (State::Disabled == m_state)
		{
			m_disabledTime.ToFileTime(&fileTime);
		}
		
		stream.Write((PByte)&m_state, sizeof(m_state));
		stream.Write((PByte)&fileTime, sizeof(fileTime));

		return key.SetValue(name, ValueType::Binary, data.GetBuffer(), (UInt32)data.GetSize());
	}

	HKEY StartupApproved::GetRoot(Startup::FileLocation location)
	{
		switch (location)
		{
		case Startup::FileLocation::CommonStartup:
			return HKEY_LOCAL_MACHINE;

		case Startup::FileLocation::UserStartup:
			return HKEY_CURRENT_USER;

		default:
			return nullptr;
		}
	}

	HKEY StartupApproved::GetRoot(Startup::RegistryLocation location)
	{
		switch (location)
		{
		case Startup::RegistryLocation::LocalMachineRun:
		case Startup::RegistryLocation::LocalMachineRun32:
			return HKEY_LOCAL_MACHINE;

		case Startup::RegistryLocation::CurrentUserRun:
			return HKEY_CURRENT_USER;

		default:
			return nullptr;
		}
	}

	const TChar* StartupApproved::GetKeyPath(Startup::FileLocation location)
	{
		return Text("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartupApproved\\StartupFolder");
	}

	const TChar* StartupApproved::GetKeyPath(Startup::RegistryLocation location)
	{
		switch (location)
		{
		case Startup::RegistryLocation::CurrentUserRun:
		case Startup::RegistryLocation::LocalMachineRun:
			return Text("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartupApproved\\Run");

		case Startup::RegistryLocation::LocalMachineRun32:
			return Text("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartupApproved\\Run32");

		default:
			return nullptr;
		}
	}

	Void StartupApproved::MoveFrom(StartupApproved& other)
	{
		if (this != &other)
		{
			m_state = other.m_state;
			m_disabledTime = std::move(other.m_disabledTime);

			other.m_state = State::Unknown;
		}
	}

	Void StartupApproved::CopyFrom(const StartupApproved& other)
	{
		if (this != &other)
		{
			m_state = other.m_state;
			m_disabledTime = other.m_disabledTime;
		}
	}

} } }