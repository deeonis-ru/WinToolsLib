#pragma once
#include "Types.h"
#include "String.h"
#include <list>

namespace WinToolsLib
{
	class Module;
	typedef std::list<Module> ModuleList;

	class Module
	{
	public:
		Module(String name, String path, PByte base, UInt32 size, Bool isKernel);
		Module(Module&& other);
		Module(const Module& other);
		~Module();

		Module& operator=(Module&& other);
		Module& operator=(const Module& other);

		const String& GetName() const;
		const String& GetPath() const;
		PByte  GetBase() const;
		UInt32 GetSize() const;
		Bool IsKernel() const;

		static ModuleList GetKernelList();
		static ModuleList GetList(UInt32 processId);

	protected:
		Void MoveFrom(Module& other);
		Void CopyFrom(const Module& other);

	protected:
		String m_name;
		String m_path;
		PByte  m_base;
		UInt32 m_size;
		Bool m_isKernel;
	};

	inline const String& Module::GetName() const
	{
		return m_name;
	}

	inline const String& Module::GetPath() const
	{
		return m_path;
	}

	inline PByte Module::GetBase() const
	{
		return m_base;
	}
	
	inline UInt32 Module::GetSize() const
	{
		return m_size;
	}

	inline Bool Module::IsKernel() const
	{
		return m_isKernel;
	}
}
