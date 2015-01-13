#pragma once
#include "Types.h"
#include "IWaitable.h"
#include "String.h"
#include "Buffer.h"
#include "Handles\Handle.h"
#include <list>

namespace WinToolsLib
{
	class Process;
	typedef std::list<Process> ProcessList;
	typedef std::list<String> ProcessModuleList;
	
	using namespace Handles;

	class Process : public IWaitable
	{
	public:
		explicit Process(UInt32 id);
		Process(UInt32 id, UInt32 parentId, String name);
		Process(UInt32 id, String name, String path);
		Process(UInt32 id, Handle&& handle);
		Process(UInt32 id, const Handle& handle);
		Process(Handle&& handle);
		Process(const Handle& handle);
		Process(Process&& other);
		Process(const Process& other);
		~Process();

		Process& operator=(Process&& other);
		Process& operator=(const Process& other);

		UInt32 GetId() const;
		UInt32 GetParentId() const;
		const String& GetName() const;
		const String& GetPath() const;
		Handle GetToken(UInt32 access) const;
		UInt32 GetExitCode() const;

		ProcessModuleList GetModuleList() const;

		Void Wait() const override;
		Void Wait(UInt32 milliseconds) const override;

		Bool IsStillActive() const;

		Void Kill(UInt32 exitCode = 0);

		static ProcessList GetList();
		static Bool IsWow64();

		static String GetFilePath();
		static String GetFilePath(UInt32 processId);
		
		static Process Run(const TChar* path, const TChar* params = nullptr, const TChar* verb = nullptr);
		static Process RunElevated(const TChar* path, const TChar* params);

		enum CreationFlags
		{
			CreateDefault = 0,
			CreateNoWindow = 0x08000000,
			CreateSuspended = 0x00000004,
			CreateAndDebug = 0x00000002
		};

		static Process Create(const TChar* commandLine, CreationFlags flags = CreateDefault);

		static Process GetCurrent();

	protected:
		Void MoveFrom(Process& other);
		Void CopyFrom(const Process& other);

	protected:
		UInt32 m_id;
		UInt32 m_parentId;
		String m_name;
		String m_path;
		Handle m_handle;

		static const UInt32 m_invalidId = -1;
	};

	inline UInt32 Process::GetId() const
	{
		return m_id;
	}

	inline UInt32 Process::GetParentId() const
	{
		return m_parentId;
	}

	inline const String& Process::GetName() const
	{
		return m_name;
	}
	
	inline const String& Process::GetPath() const
	{
		return m_path;
	}
}
