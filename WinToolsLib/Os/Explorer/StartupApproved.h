#pragma once
#include "..\..\Types.h"
#include "..\..\Time.h"
#include "..\..\String.h"
#include "..\..\Registry\Key.h"
#include "..\StartupLocation.h"

#include <Windows.h>

namespace WinToolsLib { namespace Os { namespace Explorer
{
	class StartupApproved
	{
		// To create an instance use ReadEntry()
		StartupApproved();

	public:
		StartupApproved(StartupApproved&& other);
		StartupApproved& operator=(StartupApproved&& other);

		StartupApproved(const StartupApproved& other);
		StartupApproved& operator=(const StartupApproved& other);

		static Bool EnableEntry(const TChar* path, Startup::FileLocation location);
		static Bool EnableEntry(const TChar* name, Startup::RegistryLocation location);

		static Bool DisableEntry(const TChar* path, Startup::FileLocation location);
		static Bool DisableEntry(const TChar* name, Startup::RegistryLocation location);

		static Bool DeleteEntry(const TChar* path, Startup::FileLocation location);
		static Bool DeleteEntry(const TChar* name, Startup::RegistryLocation location);
		
		static StartupApproved ReadEntry(const TChar* path, Startup::FileLocation location);
		static StartupApproved ReadEntry(const TChar* name, Startup::RegistryLocation location);

		Bool Read(const TChar* name, const Registry::Key& key);
		Bool Write(const TChar* name, Registry::Key& key) const;

		enum class State
		{
			Unknown = 0,
			Enabled = 2,
			Disabled = 3
		};
		State GetState() const;
		const Time& GetDisabledTime() const;

		Void SetState(State state);
		Void SetDisabledTime(Time disabledTime);

	protected:
		static HKEY GetRoot(Startup::FileLocation location);
		static HKEY GetRoot(Startup::RegistryLocation location);
		
		static const TChar* GetKeyPath(Startup::FileLocation location);
		static const TChar* GetKeyPath(Startup::RegistryLocation location);

		Void MoveFrom(StartupApproved& other);
		Void CopyFrom(const StartupApproved& other);

	private:
		State m_state;
		Time  m_disabledTime;
	};

	inline StartupApproved::State StartupApproved::GetState() const
	{
		return m_state;
	}

	inline const Time& StartupApproved::GetDisabledTime() const
	{
		return m_disabledTime;
	}

	inline Void StartupApproved::SetState(State state)
	{
		m_state = state;
	}

	inline Void StartupApproved::SetDisabledTime(Time disabledTime)
	{
		m_disabledTime = std::move(disabledTime);
	}

} } }