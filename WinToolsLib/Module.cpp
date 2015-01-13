#include "Module.h"
#include "Exception.h"
#include "Process.h"
#include "System.h"
#include "FileSystem\Path.h"
#include "Handles\Handle.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <algorithm>
#include <functional>

namespace WinToolsLib
{
	Module::Module(String name, String path, PByte base, UInt32 size, Bool isKernel) :
		m_name(std::move(name)),
		m_path(std::move(path)),
		m_base(base),
		m_size(size),
		m_isKernel(isKernel)
	{
	}

	Module::Module(Module&& other)
	{
		MoveFrom(other);
	}

	Module::Module(const Module& other)
	{
		CopyFrom(other);
	}

	Module::~Module()
	{
	}

	Module& Module::operator=(Module&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Module& Module::operator=(const Module& other)
	{
		CopyFrom(other);
		return *this;
	}

	ModuleList Module::GetList(UInt32 processId)
	{
		Handle hSnapshot = ::CreateToolhelp32Snapshot(
			TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32, processId);

		if (!hSnapshot.IsValid())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		MODULEENTRY32 moduleEntry = {0};
		moduleEntry.dwSize = sizeof(moduleEntry);

		BOOL success = ::Module32First(hSnapshot, &moduleEntry);
		if (!success)
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		ModuleList list;
		while (success)
		{
			//
			// Native processes on Windows XP are known to have weird paths
			//
			auto path = FileSystem::Path::NtToWin32(moduleEntry.szExePath, False);

			list.push_back(Module(
				moduleEntry.szModule,
				path,
				moduleEntry.modBaseAddr,
				moduleEntry.modBaseSize,
				False));

			success = ::Module32Next(hSnapshot, &moduleEntry);
		}

		return list;
	}

	Void Module::MoveFrom(Module& other)
	{
		if (this != &other)
		{
			m_name = std::move(other.m_name);
			m_path = std::move(other.m_path);
			m_base = other.m_base;
			m_size = other.m_size;
			m_isKernel = other.m_isKernel;
		}
	}
	
	Void Module::CopyFrom(const Module& other)
	{
		if (this != &other)
		{
			m_name = other.m_name;
			m_path = other.m_path;
			m_base = other.m_base;
			m_size = other.m_size;
			m_isKernel = other.m_isKernel;
		}
	}
}
