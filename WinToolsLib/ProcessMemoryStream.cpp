#include "ProcessMemoryStream.h"
#include "CommonMemoryStream.h"
#include "Exception.h"

namespace WinToolsLib
{
	ProcessMemoryStream::ProcessMemoryStream(UInt32 processId) 
	{
		m_hProcess = ::OpenProcess(
			PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
			FALSE,
			processId);

		if (!m_hProcess.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}

	Void ProcessMemoryStream::Read(PByte buffer, SizeT bytes)
	{
		SIZE_T bytesRead = 0;
		
		BOOL success = ::ReadProcessMemory(
			m_hProcess,
			(LPCVOID)m_position,
			buffer,
			bytes,
			&bytesRead);

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		m_position += bytes;
	}

	Void ProcessMemoryStream::Write(PByte buffer, SizeT bytes)
	{
		DWORD oldProtect = 0;

		BOOL success = ::VirtualProtectEx(
			m_hProcess,
			(LPVOID)m_position,
			bytes,
			PAGE_READWRITE,
			&oldProtect);

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		SIZE_T bytesWritten = 0;

		success = ::WriteProcessMemory(
			m_hProcess,
			(LPVOID)m_position,
			buffer,
			bytes,
			&bytesWritten);

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		m_position += bytes;

		success = ::VirtualProtectEx(
			m_hProcess,
			(LPVOID)m_position,
			bytes,
			oldProtect,
			&oldProtect);

		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}
	}
}