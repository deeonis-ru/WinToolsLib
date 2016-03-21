#include "Process.h"
#include "..\Exception.h"
#include "..\System.h"
#include "..\FileSystem\Path.h"
#include "..\Handles\Handle.h"

#include <TlHelp32.h>

namespace WinToolsLib { namespace Process
{
	Thread InjectDll(Process& process, const String& dllPath)
	{
		const auto size = (dllPath.GetSize() + 1) * sizeof(TChar);
		auto remotePath = ::VirtualAllocEx(
			process.m_handle,
			NULL,
			size,
			MEM_COMMIT,
			PAGE_READWRITE);
		if (!remotePath)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		if (!::WriteProcessMemory(process.m_handle, remotePath, dllPath.GetBuffer(), size, NULL))
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		auto kernel32 = ::GetModuleHandle(_T("Kernel32"));
		if (!kernel32)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

#ifdef _UNICODE
		auto loadLibrary = "LoadLibraryW";
#else
		auto loadLibrary = "LoadLibraryA";
#endif

		return process.CreateRemoteThread(
			(LPTHREAD_START_ROUTINE) ::GetProcAddress(kernel32, loadLibrary),
			remotePath);
		// TODO: We must free memory at the end of thread
	}

	Process::Process(UInt32 id) :
		m_id(id),
		m_parentId(m_invalidId)
	{
	}

	Process::Process(UInt32 id, UInt32 parentId, String name) :
		m_id(id),
		m_parentId(parentId),
		m_name(std::move(name))
	{
		m_path = GetFilePath(id);
	}

	Process::Process(UInt32 id, String name, String path) :
		m_id(id),
		m_parentId(m_invalidId),
		m_name(std::move(name)),
		m_path(std::move(path))
	{
	}

	Process::Process(UInt32 id, Handle&& handle) :
		m_id(id),
		m_parentId(m_invalidId),
		m_handle(std::move(handle))
	{
	}

	Process::Process(UInt32 id, const Handle& handle) :
		m_id(id),
		m_parentId(m_invalidId),
		m_handle(handle)
	{
	}

	Process::Process(Handle&& handle) :
		m_id(m_invalidId),
		m_handle(std::move(handle))
	{
		m_id = ::GetProcessId(m_handle);
	}

	Process::Process(const Handle& handle) :
		m_id(m_invalidId),
		m_handle(handle)
	{
		m_id = ::GetProcessId(m_handle);
	}

	Process::Process(Process&& other)
	{
		MoveFrom(other);
	}

	Process::Process(const Process& other)
	{
		CopyFrom(other);
	}

	Process::~Process()
	{
	}

	Process& Process::operator=(Process&& other)
	{
		MoveFrom(other);
		return *this;
	}
	
	Process& Process::operator=(const Process& other)
	{
		CopyFrom(other);
		return *this;
	}

	Handle Process::GetToken(UInt32 access) const
	{
		HANDLE hToken = NULL;
		auto success = ::OpenProcessToken(m_handle, access, &hToken);

		Handle handle(hToken);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return handle;
	}

	UInt32 Process::GetExitCode() const
	{
		DWORD exitCode = 0;

		auto success = ::GetExitCodeProcess(m_handle, &exitCode);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return static_cast<UInt32>(exitCode);
	}

	ProcessModuleList Process::GetModuleList() const
	{
		Handle hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_id);
		if (hModuleSnap.IsValid())
		{
			MODULEENTRY32 moduleEntry = { 0 };
			moduleEntry.dwSize = sizeof(moduleEntry);

			if (!::Module32First(hModuleSnap, &moduleEntry))
				return ProcessModuleList();

			ProcessModuleList moduleList;
			do
			{
				moduleList.push_back(moduleEntry.szExePath);
			} 
			while (::Module32Next(hModuleSnap, &moduleEntry));
			return std::move(moduleList);
		}
		return ProcessModuleList();
	}

	Void Process::Wait() const
	{
		Wait(INFINITE);
	}

	Void Process::Wait(UInt32 milliseconds) const
	{
		Handle handle(m_handle);
		if (!handle.IsValid())
		{
			handle = ::OpenProcess(SYNCHRONIZE, False, m_id);
			if (!handle.IsValid())
				THROW_INVALID_HANDLE_EXCEPTION();
		}

		DWORD result = ::WaitForSingleObject(handle, milliseconds);
		if (WAIT_FAILED == result)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Bool Process::IsStillActive() const
	{
		Handle handle = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, m_id);
		if (!handle.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		DWORD exitCode = 0;

		BOOL success = ::GetExitCodeProcess(handle, &exitCode);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		if (STILL_ACTIVE == exitCode)
		{
			return True;
		}
		return False;
	}

	Void Process::Kill(UInt32 exitCode)
	{
		auto handle = Handle(::OpenProcess(PROCESS_TERMINATE, FALSE, m_id));
		if (handle.IsNull())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		auto success = ::TerminateProcess(handle, exitCode);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Thread Process::CreateRemoteThread(LPTHREAD_START_ROUTINE proc, PVOID param, CreationThreadFlags flags) const
	{
		auto currentProcId = ::GetCurrentProcess();
		// TODO: throw exception if currentProcId == m_handle

		DWORD threadId = 0;
		Handle threadHandle = ::CreateRemoteThread(
			m_handle,
			NULL,
			0,
			proc,
			param,
			(DWORD)flags,
			&threadId);
		if (threadHandle.IsNull())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return Thread(threadId, std::move(threadHandle));
	}

	ProcessList Process::GetList()
	{
		Handle hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!hSnapshot.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		PROCESSENTRY32 processEntry = {0};
		processEntry.dwSize = sizeof(processEntry);

		BOOL success = ::Process32First(hSnapshot, &processEntry);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		ProcessList list;
		while (success)
		{
#ifdef _DEBUG
			//if (_wcsicmp(processEntry.szExeFile, L"winword.exe") == 0 ||
			//	_wcsicmp(processEntry.szExeFile, L"iexplore.exe") == 0 ||
			//	wcscmp(processEntry.szExeFile, L"firefox.exe") == 0 ||
			//	wcsncmp(processEntry.szExeFile, L"FlashPlayerPlugin", 17) == 0 ||
			//	wcscmp(processEntry.szExeFile, L"AccessEnum.exe") == 0 ||
			//	wcscmp(processEntry.szExeFile, L"TSVNCache.exe") == 0 ||
			//	wcscmp(processEntry.szExeFile, L"plugin-container.exe") == 0)
#endif
			list.push_back(Process(
				processEntry.th32ProcessID,
				processEntry.th32ParentProcessID,
				processEntry.szExeFile));

			success = ::Process32Next(hSnapshot, &processEntry);
		}

		return list;
	}

	Bool Process::IsWow64()
	{
		typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

		auto kernel32 = ::GetModuleHandle(Text("kernel32"));
		if (!kernel32)
		{
			return False;
		}

		auto fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(kernel32, "IsWow64Process");
		if (!fnIsWow64Process)
		{
			return False;
		}

		BOOL isWow64 = FALSE;
		if (!fnIsWow64Process(::GetCurrentProcess(), &isWow64))
		{
			return False;
		}

		return TRUE == isWow64;
	}

	String Process::GetFilePath()
	{
		TCHAR pszExe[MAX_PATH] = {0};
		
		BOOL success = ::GetModuleFileName(NULL, pszExe, MAX_PATH);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return String(pszExe);
	}

	String Process::GetFilePath(UInt32 processId)
	{
		Handle hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
		if (hModuleSnap.IsValid())
		{	
			MODULEENTRY32 moduleEntry = {0};
			moduleEntry.dwSize = sizeof(moduleEntry);
			
			if (::Module32First(hModuleSnap, &moduleEntry)) 
			{
				//
				// Native processes on Windows XP are known to have weird paths
				//
				return FileSystem::Path::NtToWin32(moduleEntry.szExePath, False);
			}
		}		
		return String();
	}

	Process Process::Run(const TChar* path, const TChar* params, const TChar* verb)
	{
		SHELLEXECUTEINFO info = {0};

		info.cbSize = sizeof(info);
		info.lpFile = path;
		info.lpParameters = params;
		info.nShow = SW_SHOWNORMAL;
		info.lpVerb = verb ? verb : Text("open");
		info.fMask = SEE_MASK_NOCLOSEPROCESS;

		BOOL success = ::ShellExecuteEx(&info);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
		
		Handle hProcess(info.hProcess);
		return Process(std::move(hProcess));
	}

	Process Process::RunElevated(const TChar* path, const TChar* params)
	{
		if (System::IsVistaOrLater())
		{
			return Process::Run(path, params, Text("runas"));
		}
		else
		{
			return Process::Run(path, params, Text("open"));
		}
	}

	Process Process::Create(const TChar* commandLine, CreationFlags flags)
	{
		STARTUPINFO startupInfo = {0};
		startupInfo.cb = sizeof(startupInfo);

		PROCESS_INFORMATION processInfo = {0};
		String buffer(commandLine);

		BOOL success = CreateProcess(
			NULL,                    // The name of the module to be executed
			buffer.GetBuffer(),      // The command line to be executed
			NULL,                    // Process handle is not inheritable
			NULL,                    // Thread handle is not inheritable
			FALSE,                   // Set handle inheritance to FALSE
			(DWORD)flags,            // Creation flags
			NULL,                    // Use parent's environment block
			NULL,                    // Use parent's starting directory 
			&startupInfo,            // Pointer to STARTUPINFO structure
			&processInfo);           // Pointer to PROCESS_INFORMATION structure
		
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		Handle hThread(processInfo.hThread);
		Handle hProcess(processInfo.hProcess);
		return Process(processInfo.dwProcessId, std::move(hProcess));
	}

	Process Process::GetCurrent()
	{
		return Process(Handle(::GetCurrentProcess()));
	}

	Void Process::MoveFrom(Process& other)
	{
		if (this != &other)
		{
			m_id = other.m_id;
			m_parentId = other.m_parentId;
			m_name = std::move(other.m_name);
			m_path = std::move(other.m_path);
			m_handle = std::move(other.m_handle);

			other.m_id = m_invalidId;
			other.m_parentId = m_invalidId;
		}
	}

	Void Process::CopyFrom(const Process& other)
	{
		if (this != &other)
		{
			m_id = other.m_id;
			m_parentId = other.m_parentId;
			m_name = other.m_name;
			m_path = other.m_path;
			m_handle = other.m_handle;
		}
	}
}}
